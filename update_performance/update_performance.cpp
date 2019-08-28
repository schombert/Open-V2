#include "common\\common.h"
#include "text_classifier/text_classifiers.h"
#include "performance_measurement/performance.h"
#include <iostream>
#include "world_state\\world_state.h"
#include "world_state\\world_state_io.h"
#include "scenario\\scenario_io.h"
#include <ppl.h>
#include "provinces\province_functions.h"

class single_world_step {
public:
	world_state& ws;

	single_world_step(world_state& s) : ws(s) {}

	int test_function() {
		world_state_non_ai_update(ws);
		ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);
		return int(ws.w.nation_s.nations.get<nation::national_debt>(nations::country_tag(10)));
	}
};

class old_fill_distance {
public:
	world_state& ws;
	std::vector<float, aligned_allocator_64<float>> distances;
	std::vector<provinces::province_tag, aligned_allocator_64<provinces::province_tag>> provs;

	old_fill_distance(world_state& s) : ws(s) {
		distances.resize(province::container_size);
		provs.resize(province::container_size);
	}

	int test_function() {
		provinces::old_path_wise_distance_cost(ws, provinces::province_tag(1), distances.data(), provs.data());
		return int(distances[89]);
	}
};

class new_fill_distance {
public:
	world_state& ws;
	std::vector<float, aligned_allocator_64<float>> distances;
	std::vector<provinces::province_tag, aligned_allocator_64<provinces::province_tag>> provs;

	new_fill_distance(world_state& s) : ws(s) {
		distances.resize(province::container_size);
		provs.resize(province::container_size);
	}

	int test_function() {
		provinces::path_wise_distance_cost(ws, provinces::province_tag(1), distances.data(), provs.data());
		return int(distances[89]);
	}
};

int main() {
	logging_object log;

	world_state* wsptr = (world_state*)_aligned_malloc(sizeof(world_state), 64);
	new (wsptr)world_state();
	world_state& ws = *wsptr;

	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();

	ready_world_state(ws);
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_save_cmp.bin", ws.w, ws);

	ws.w.local_player_nation = nations::country_tag{};

	{
		test_object<20, 100, old_fill_distance> to(ws);
		std::cout << to.log_function(log, "original pathwise distance") << std::endl;
	}

	{
		test_object<20, 100, new_fill_distance> to(ws);
		std::cout << to.log_function(log, "variant pathwise distance") << std::endl;
	}
	
	{
		// test_object<20, 100, single_world_step> to(ws);
		// std::cout << to.log_function(log, "world state 100 steps update") << std::endl;
	}

	_aligned_free(wsptr);

	return 0;
}

