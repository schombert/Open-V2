#include "performance.h"
#include "db_wrapper\\simple_db.hpp"
#include <memory>
#include <chrono>
#include <ctime>

DB_HIDDEN_KEY(h_key);
DB_UINT(avg_microseconds);
DB_UINT(min_microseconds);
DB_UINT(max_microseconds);
DB_UINT(outer_loops_count);
DB_UINT(inner_loops_count);
DB_TEXT(date);
DB_TEXT(test_name);
DB_TABLE(performance_logs, db_h_key, db_avg_microseconds, db_min_microseconds, db_max_microseconds, db_outer_loops_count, db_inner_loops_count, db_date, db_test_name);

class private_db_type : public simple_db <db_performance_logs> {
public:
	private_db_type() {}
};

logging_object::logging_object() : private_db(std::make_unique<private_db_type>()) {
	private_db->open_or_create("F:\\VS2007Projects\\open_v2_test_data\\perf.db");
	private_db->begin_transaction();
}

logging_object::~logging_object() {
	private_db->end_transaction();
	private_db->save_and_close("F:\\VS2007Projects\\open_v2_test_data\\perf.db");
}

void logging_object::log_results(const intermediate_results& results, uint32_t outer_loops, uint32_t inner_loops, const char* test_name) {
	auto inserter = private_db->insert_into<db_performance_logs>();
	inserter.set_column<db_avg_microseconds>(results.total_microseconds / results.number_measurements);
	inserter.set_column<db_min_microseconds>(results.minimum_microseconds);
	inserter.set_column<db_max_microseconds>(results.maximum_microseconds);
	inserter.set_column<db_outer_loops_count>(outer_loops);
	inserter.set_column<db_inner_loops_count>(inner_loops);

	const auto raw_time = std::time(nullptr);
	struct tm ltime;
	localtime_s(&ltime, &raw_time);

	char formatted_time[256] = "";
	strftime(formatted_time, 256, "%y-%m-%d:%H:%M", &ltime);

	inserter.set_column<db_date>(formatted_time);
	inserter.set_column<db_test_name>(test_name);
	inserter.execute();
}
