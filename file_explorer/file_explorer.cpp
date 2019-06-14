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
#include "provinces\\province_functions.hpp"
#include "nations\\nations_functions.hpp"
#include "nations\\nations_io.h"
#include "military\\military_io.h"
#include "world_state\\world_state_io.h"
#include "ideologies\\ideologies_functions.h"
#include "population\\population_functions.hpp"
#include "economy\\economy_functions.hpp"
#include "modifiers\\modifier_functions.h"
#include "events\\event_functions.h"
#include <random>

#undef max
#undef min
// #define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

int main(int, char**);



struct gui_window_handler {
	world_state& s;
	std::string shadows_file;
	std::string map_bg_file;
	const directory root;
	
	Eigen::Vector3f interest = Eigen::Vector3f::UnitX();
	bool map_dragging = false;
	std::pair<int32_t, int32_t> map_drag_start;

	gui_window_handler(world_state& snm, std::string const& shadows, std::string const& bg, directory const& d) : s(snm), shadows_file(shadows), map_bg_file(bg), root(d) {
		s.w.map.colors.init_color_data(static_cast<uint32_t>(s.s.province_m.province_container.size()));
	}

	template<typename T>
	void operator()(const T&, ui::window_base& w) const {
		// do nothing;
	}

	void operator()(const ui::creation&, ui::window_base& win) {
		s.w.init_gui_objects(s);
		sound::init_sound_manager(s.s.sound_m, root, win.get_handle());
		if(s.s.sound_m.first_music != -1)
			s.s.sound_m.play_music(s.s.sound_m.first_music, s.s.settings.music_volume * s.s.settings.master_volume);
		else
			sound::play_new_track(s);

		win.maximize();
		//win.make_fullscreen();
		map_mode::change_mode(s, map_mode::type::political);
	}
	void operator()(const ui::music_finished&, ui::window_base&) {
		if(s.s.sound_m.music_finished())
			sound::play_new_track(s);
	}
	void operator()(const ui::resize& r, ui::window_base&) {
		s.w.gui_m.on_resize(r);
		s.w.map.resize(s.w.gui_m.root.size.x, s.w.gui_m.root.size.y);
		s.w.topbar_w.resize_topbar(s.w.gui_m);
	}

	void operator()(const ui::lbutton_down& m, ui::window_base&) {
		if (!s.w.gui_m.on_lbutton_down(s, m)) {
			map_dragging = true;
			map_drag_start = std::make_pair(m.x, m.y);
			interest = s.w.map.get_vector_for(s.w.map.normalize_screen_coordinates(m.x, m.y, s.w.gui_m.width(), s.w.gui_m.height()));
		}
	}
	void operator()(const ui::rbutton_down& m, ui::window_base&) {
		if(!s.w.gui_m.on_rbutton_down(s, m)) {
			///
		}
	}
	void operator()(const ui::lbutton_up& m, ui::window_base&) {
		s.w.gui_m.on_lbutton_up(s, m);
		if(map_dragging) {
			map_dragging = false;

			if(std::abs(map_drag_start.first - m.x) < 5 && std::abs(map_drag_start.second - m.y) < 5) {
				auto map_coord = s.w.map.map_coordinates_from_screen(s.w.map.normalize_screen_coordinates(m.x, m.y, s.w.gui_m.width(), s.w.gui_m.height()));
				auto id = s.s.province_m.province_map_data[size_t(map_coord.first + map_coord.second * s.s.province_m.province_map_width)];
				
				map_mode::on_selection(s, provinces::province_tag(id));

				if(is_valid_index(provinces::province_tag(id)) && id != 0 && id < s.s.province_m.first_sea_province) {
					if(auto sid = provinces::province_state(s, provinces::province_tag(id)); sid) {
						if(s.w.nation_s.states.is_valid_index(sid) && s.w.trade_w.selected_state != sid) {
							s.w.trade_w.selected_state = sid;
							s.w.trade_w.update(s.w.gui_m);
						}
					}
				} else {
					s.w.province_w.hide_province_window(s.w.gui_m);
				}
			} 
		}
	}
	void operator()(const ui::key_down& m, ui::window_base&) {
		if(!s.w.gui_m.on_keydown(s, m)) {
			if(m.keycode == virtual_key::ESCAPE) {
				sound::play_interface_sound(s, s.s.sound_m.click_sound);
				s.w.menu_w.show_menu_window(s);
			} else if(m.keycode == virtual_key::NUMPAD1 || m.keycode == virtual_key::NUM_1) {
				map_mode::change_mode(s, map_mode::type::political);
			} else if(m.keycode == virtual_key::NUMPAD2 || m.keycode == virtual_key::NUM_2) {
				map_mode::change_mode(s, map_mode::type::distance);
			} else if(m.keycode == virtual_key::NUMPAD3 || m.keycode == virtual_key::NUM_3) {
				map_mode::change_mode(s, map_mode::type::prices);
			} else if(m.keycode == virtual_key::NUMPAD4 || m.keycode == virtual_key::NUM_4) {
				map_mode::change_mode(s, map_mode::type::purchasing);
			} else if(m.keycode == virtual_key::NUMPAD5 || m.keycode == virtual_key::NUM_5) {
				map_mode::change_mode(s, map_mode::type::production);
			}
		}
	}
	void operator()(const ui::scroll& ss, ui::window_base&) {
		if (!s.w.gui_m.on_scroll(s, ss)) {
			if(s.s.settings.zoom_setting == scenario::zoom_type::to_center) {
				s.w.map.rescale_by(float(pow(2, ss.amount / 2.0f)));
			} else {
				auto const old_cursor = s.w.map.get_vector_for(s.w.map.normalize_screen_coordinates(ss.x,ss.y, s.w.gui_m.width(), s.w.gui_m.height()));
				s.w.map.rescale_by(float(pow(2, ss.amount / 2.0f)));
				auto const new_cursor = s.w.map.get_unrotated_vector_for(s.w.map.normalize_screen_coordinates(ss.x, ss.y, s.w.gui_m.width(), s.w.gui_m.height()));
				s.w.map.move_vector_to(old_cursor, new_cursor);
			}
		}
	}
	void operator()(const ui::mouse_drag& m, ui::window_base&) {
		if (!map_dragging) {
			s.w.gui_m.on_mouse_drag(s, m);
		} else {
			Eigen::Vector3f mouse_over = s.w.map.get_unrotated_vector_for(s.w.map.normalize_screen_coordinates(map_drag_start.first + m.x, map_drag_start.second + m.y, s.w.gui_m.width(), s.w.gui_m.height()));
			s.w.map.move_vector_to(interest, mouse_over);
			s.w.bottombar_w.update_location(s);
		}
	}
	void operator()(const ui::mouse_move& m, ui::window_base&) {
		if(!s.w.gui_m.on_mouse_move(s, m)) {
			auto map_coord = s.w.map.map_coordinates_from_screen(s.w.map.normalize_screen_coordinates(m.x, m.y, s.w.gui_m.width(), s.w.gui_m.height()));
			auto id = s.s.province_m.province_map_data[size_t(map_coord.first + map_coord.second * s.s.province_m.province_map_width)];
			map_mode::on_mouse_over(s, provinces::province_tag(id));
		}
	}
	void operator()(const ui::text_event& t, ui::window_base&) {
		if(!s.w.gui_m.on_text(s, t)) {
			if(t.text == u'.') {
				s.w.single_step_pending.store(true, std::memory_order_release);
			}
		}
	}
	void initialize_graphics(graphics::open_gl_wrapper& ogl) {
		s.s.gui_m.fonts.load_fonts(ogl);
	
		//map.initialize(ogl, s.province_m.province_map_data.data(), s.province_m.province_map_width, s.province_m.province_map_height, 0.0f, -1.2f, 1.2f);
		s.w.map.initialize(ogl, s.s, shadows_file, map_bg_file, s.s.province_m.province_map_data.data(), s.s.province_m.province_map_width, s.s.province_m.province_map_height, 0.0f, 1.57f, -1.57f);
		s.w.map.resize(s.w.gui_m.root.size.x, s.w.gui_m.root.size.y);
		s.w.map.init_province_ui(
			s.s.province_m.province_container.get_row<province::centroid_2d>(),
			s.s.province_m.province_container.size(), [_this = this]() { _this->s.w.gui_m.flag_minimal_update(); });
		s.w.bottombar_w.update_location(s);
	}

	void on_idle(ui::window_base& win) {
		map_mode::update_map_colors(s);

		if (s.w.gui_m.check_and_clear_update()) {
			ui::update(s);
		} else if (s.w.gui_m.check_and_clear_minimal_update()) {
			ui::minimal_update(s);
		}

		if(s.w.end_game.load(std::memory_order_acquire))
			win.close_window();
		if(s.s.settings.window_mode == 0)
			win.remove_fullscreen();
		else
			win.make_fullscreen();
	}

	void render(graphics::open_gl_wrapper& ogl) {
		s.w.map.render(ogl, s);
		s.w.map.update_province_ui_positions();
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
		provinces::read_adjacnencies_file(adj_map, fs.get_root(), s1);
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

	world_state* wsptr = (world_state*)_aligned_malloc(sizeof(world_state), 64);
	new (wsptr)world_state();
	world_state& ws = *wsptr;

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

		ws.w.province_s.province_state_container.for_each([&ws](provinces::province_tag p) {
			if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::rgo_production>(p)))
				economy::match_rgo_worker_type(ws, p);
		});

		provinces::recalculate_province_demographics(ws);
		nations::recalculate_state_nation_demographics(ws);

		provinces::set_base_rgo_size(ws);

		ws.w.province_s.province_state_container.for_each([&ws](provinces::province_tag p) {
			if(bool(ws.w.province_s.province_state_container.get<province_state::owner>(p))
				&& bool(ws.w.province_s.province_state_container.get<province_state::state_instance>(p))) {
				economy::update_rgo_employment(ws, p);
			}
		});

		economy::init_factory_employment(ws);
		economy::init_artisan_producation(ws);

		population::default_initialize_world_issues_and_ideology(ws);

		provinces::recalculate_province_demographics(ws);
		nations::recalculate_state_nation_demographics(ws);
		provinces::ready_initial_province_statistics(ws);

		nations::fix_capitals(ws);

		events::execute_decision_set(decisions, ws);

		economy::set_initial_money(ws);

		ws.w.local_player_nation = nations::country_tag(1);

		provinces::add_province_modifier(
			ws,
			provinces::province_tag(78),
			modifiers::provincial_modifier_tag(1));
		provinces::add_timed_province_modifier(
			ws,
			provinces::province_tag(78),
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
		modifiers::add_static_modifier_to_nation(ws, ws.w.local_player_nation, nmod);
	}
	{
		//auto mod_thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "army_tech_school");
		//auto nmod = ws.s.modifiers_m.named_national_modifiers_index[mod_thandle];
		//ws.w.nation_s.nations.set<nation::tech_school>(ws.w.local_player_nation, nmod);

		
	}
	{
		auto thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "strategic_mobility");
		auto tech = ws.s.technology_m.named_technology_index[thandle];
		ws.w.nation_s.nations.set<nation::current_research>(ws.w.local_player_nation, tech);
		ws.w.nation_s.nations.set<nation::research_points>(ws.w.local_player_nation, 7180.0f);
	}
	{
		ws.w.current_crisis.type = current_state::crisis_type::liberation;
		ws.w.current_crisis.primary_attacker = nations::country_tag(0);
		ws.w.current_crisis.primary_defender = nations::country_tag(1);
		ws.w.current_crisis.on_behalf_of = nations::country_tag(2);
		ws.w.current_crisis.target = nations::country_tag(1);

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
		ws.w.nation_s.nations.set<nation::cb_construction_progress>(ws.w.local_player_nation, 0.7f);
		ws.w.nation_s.nations.set<nation::cb_construction_target>(ws.w.local_player_nation, nations::country_tag(6));
		ws.w.nation_s.nations.set<nation::cb_construction_type>(ws.w.local_player_nation, military::cb_type_tag(4));

		ws.w.nation_s.nations.set<nation::cb_construction_progress>(nations::country_tag(3), 0.6f);
		ws.w.nation_s.nations.set<nation::cb_construction_target>(nations::country_tag(3), nations::country_tag(3));
		ws.w.nation_s.nations.set<nation::cb_construction_type>(nations::country_tag(3),military::cb_type_tag(2));

		ws.w.nation_s.nations.set<nation::cb_construction_progress>(nations::country_tag(7), 0.3f);
		ws.w.nation_s.nations.set<nation::cb_construction_target>(nations::country_tag(7), nations::country_tag(1));
		ws.w.nation_s.nations.set<nation::cb_construction_type>(nations::country_tag(7), military::cb_type_tag(1));
		ws.w.nation_s.nations.set<nation::cb_construction_discovered>(nations::country_tag(7), true);
	}
	{
		//auto srange = get_range(ws.w.nation_s.state_arrays, ws.w.local_player_nation->member_states);
		//(srange.first)->state->project.type = economy::pop_project_type::factory;
		//(srange.first)->state->project.factory_type = economy::factory_type_tag(1);

		//(srange.first + 3)->state->project.type = economy::pop_project_type::railroad;
		//(srange.first + 3)->state->project.location = provinces::province_tag(6);
	}

	std::cout << ws.w.nation_s.nations.get<nation::treasury>(ws.w.local_player_nation) << std::endl;

	ws.w.current_date = date_tag(to_index(ws.w.current_date) - 2);
	economy::economy_demand_adjustment_tick(ws);

	std::cout << ws.w.nation_s.nations.get<nation::treasury>(ws.w.local_player_nation) << std::endl;

	ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);
	economy::economy_demand_adjustment_tick(ws);
	
	std::cout << ws.w.nation_s.nations.get<nation::treasury>(ws.w.local_player_nation) << std::endl;


	ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);

	std::cout << ws.w.nation_s.nations.get<nation::treasury>(ws.w.local_player_nation) << std::endl;


	provinces::recalculate_province_demographics(ws);
	nations::recalculate_state_nation_demographics(ws);

	init_tooltip_window(ws.s.gui_m, ws.w.gui_m);
	ws.w.gui_m.on_resize(ui::resize{ 1024ui32, 800ui32 });

	std::cout << ws.w.nation_s.nations.get<nation::treasury>(ws.w.local_player_nation) << std::endl;

	std::thread update_thread([&ws](){
		world_state_update_loop(ws);
	});

	{
		const auto map_dir = fs.get_root().get_directory(u"\\map");
		const auto gfx_dir = fs.get_root().get_directory(u"\\gfx");
		const auto interface_dir = gfx_dir.get_directory(u"\\interface");

		auto map_peek = map_dir.peek_file(u"shadows.png");
		std::string shadows = map_peek ? UTF16toUTF8(map_peek->file_path() + u'\\' + map_peek->file_name()) : std::string();

		auto bg_peek = interface_dir.peek_file(u"background_map.dds");
		std::string map_bg = bg_peek ? UTF16toUTF8(bg_peek->file_path() + u'\\' + bg_peek->file_name()) : std::string();

		ui::window<gui_window_handler> test_window(1024, 800, ws, shadows, map_bg, fs.get_root());

		std::cout << "test window created" << std::endl;
		getchar();

		test_window.close_window();
	}
	ws.w.end_game.store(true, std::memory_order_release);

	getchar();

	update_thread.join();

	_aligned_free(wsptr);
	return 0;
}

