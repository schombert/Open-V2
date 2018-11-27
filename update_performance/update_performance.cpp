#include "common\\common.h"
#include "text_classifier/text_classifiers.h"
#include "performance_measurement/performance.h"
#include <iostream>
#include "world_state\\world_state.h"
#include "world_state\\world_state_io.h"
#include "scenario\\scenario_io.h"
#include <ppl.h>

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

int main() {
	logging_object log;

	world_state ws;

	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();

	ready_world_state(ws);
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_save_cmp.bin", ws.w, ws);

	test_object<20, 100, single_world_step> to(ws);
	std::cout << to.log_function(log, "world state 100 steps update") << std::endl;

	return 0;
}

