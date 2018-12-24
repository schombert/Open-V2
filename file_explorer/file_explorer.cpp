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
#include <random>

#undef max
#undef min
// #define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

int main(int, char**);



struct gui_window_handler {
	world_state& s;
	std::string shadows_file;

	graphics::map_display map;
	Eigen::Vector3f interest = Eigen::Vector3f::UnitX();
	bool map_dragging = false;
	std::pair<int32_t, int32_t> map_drag_start;

	gui_window_handler(world_state& snm, std::string const& shadows) : s(snm), shadows_file(shadows) {
		map.colors.init_color_data(static_cast<uint32_t>(s.s.province_m.province_container.size()));
	}

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
		s.w.gui_m.on_lbutton_up(s, m);
		if(map_dragging) {
			map_dragging = false;

			if(std::abs(map_drag_start.first - m.x) < 5 && std::abs(map_drag_start.second - m.y) < 5) {
				auto map_coord = map.map_coordinates_from_screen(map.state.normalize_screen_coordinates(m.x, m.y, s.w.gui_m.width(), s.w.gui_m.height()));
				auto id = s.s.province_m.province_map_data[size_t(map_coord.first + map_coord.second * s.s.province_m.province_map_width)];
				s.w.province_w.show_province_window(s.w.gui_m, provinces::province_tag(id));
				s.w.map_view.selected_province = provinces::province_tag(id);
				if(is_valid_index(provinces::province_tag(id))) {
					if(auto sid = provinces::province_state(s, provinces::province_tag(id)); sid) {
						if(s.w.nation_s.states.is_valid_index(sid) && s.w.map_view.selected_state != sid) {
							s.w.map_view.selected_state = sid;
							s.w.trade_w.selected_state = sid;
							s.w.trade_w.update(s.w.gui_m);
						}
					}
					if(auto n = provinces::province_owner(s, provinces::province_tag(id)); n) {
						if(auto nid = n; s.w.nation_s.nations.is_valid_index(nid))
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
			s.w.single_step_pending.store(true, std::memory_order_release);
		} else {
			s.w.gui_m.on_text(s, t);
		}
	}
	void initialize_graphics(graphics::open_gl_wrapper& ogl) {
		s.s.gui_m.fonts.load_fonts(ogl);
	
		//map.initialize(ogl, s.province_m.province_map_data.data(), s.province_m.province_map_width, s.province_m.province_map_height, 0.0f, -1.2f, 1.2f);
		map.initialize(ogl, shadows_file, s.s.province_m.province_map_data.data(), s.s.province_m.province_map_width, s.s.province_m.province_map_height, 0.0f, 1.57f, -1.57f);
		map.state.resize(s.w.gui_m.width(), s.w.gui_m.height());
	}

	void on_idle(ui::window_base& win) {
		graphics::update_map_colors(map, s);

		if (s.w.gui_m.check_and_clear_update()) {
			ui::update(s);
		} else if (s.w.gui_m.check_and_clear_minimal_update()) {
			ui::minimal_update(s);
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
		auto mod_thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "army_tech_school");
		auto nmod = ws.s.modifiers_m.named_national_modifiers_index[mod_thandle];
		ws.w.nation_s.nations.set<nation::tech_school>(ws.w.local_player_nation, nmod);

		
	}
	{
		auto thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "strategic_mobility");
		auto tech = ws.s.technology_m.named_technology_index[thandle];
		ws.w.nation_s.nations.set<nation::current_research>(ws.w.local_player_nation, tech);
		ws.w.nation_s.nations.set<nation::research_points>(ws.w.local_player_nation, 1500.0f);
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

	ws.w.current_date = date_tag(to_index(ws.w.current_date) - 2);
	economy::economy_demand_adjustment_tick(ws);
	ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);
	economy::economy_demand_adjustment_tick(ws);
	ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);

	provinces::recalculate_province_demographics(ws);
	nations::recalculate_state_nation_demographics(ws);

	init_tooltip_window(ws.s.gui_m, ws.w.gui_m);
	ws.w.gui_m.on_resize(ui::resize{ 850ui32, 650ui32 });

	std::thread update_thread([&ws](){
		world_state_update_loop(ws);
	});

	{
		const auto map_dir = fs.get_root().get_directory(u"\\map");
		
		auto map_peek = map_dir.peek_file(u"shadows.png");
		std::string shadows = map_peek ? UTF16toUTF8(map_peek->file_path() + u'\\' + map_peek->file_name()) : std::string();

		ui::window<gui_window_handler> test_window(850, 650, ws, shadows);

		std::cout << "test window created" << std::endl;
		getchar();

		test_window.close_window();
	}
	ws.w.end_game.store(true, std::memory_order_release);

	getchar();

	update_thread.join();
	return 0;
}

