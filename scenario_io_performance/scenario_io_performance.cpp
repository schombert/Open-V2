#include "common\\common.h"
#include "performance_measurement\\performance.h"
#include <ppl.h>
#include "scenario\\scenario_io.h"
#include <iostream>

class scenario_read {
public:
	scenario_read() {}

	int test_function() {
		file_system fs;
		fs.set_root(u"D:\\programs\\V2");

		scenario::scenario_manager s;
		scenario::read_scenario(s, fs.get_root());

		return (int)s.economy_m.goods_count;
	}
};

class scenario_deserialize_st {
public:
	std::vector<std::byte> const& source;

	scenario_deserialize_st(std::vector<std::byte> const& s) : source(s) {}

	int test_function() {
		scenario::scenario_manager s;

		std::byte const* optr = source.data();
		serialization::deserialize(optr, s);

		return (int)s.economy_m.goods_count;
	}
};

class scenario_deserialize_mt {
public:
	std::vector<std::byte> const& source;

	scenario_deserialize_mt(std::vector<std::byte> const& s) : source(s) {}

	int test_function() {
		scenario::scenario_manager s;

		concurrency::task_group tg;

		std::byte const* optr = source.data();
		serialization::deserialize(optr, s, tg);

		tg.wait();

		return (int)s.economy_m.goods_count;
	}
};

int main() {
	logging_object log;
	{
		scenario::scenario_manager s;
		file_system fs;
		fs.set_root(u"D:\\programs\\V2");

		scenario::read_scenario(s, fs.get_root());

		const auto s_size = serialization::serialize_size(s);
		std::vector<std::byte> sdata(s_size);
		auto ptr = sdata.data();
		serialization::serialize(ptr, s);

		test_object<10, 10, scenario_deserialize_st> st(sdata);
		std::cout << st.log_function(log, "scenario deserialization: single threaded") << std::endl;

		test_object<10, 10, scenario_deserialize_mt> mt(sdata);
		std::cout << mt.log_function(log, "scenario deserialization: multi threaded") << std::endl;
	}
	return 0;
}

