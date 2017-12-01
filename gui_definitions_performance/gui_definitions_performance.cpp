#include "performance_measurement\\performance.h"
#include "gui_definitions\\gui_definitions.h"
#include <iostream>

auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_font_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_gobj_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_sound_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}

class gui_definitions_perf  {
public:
	gui_definitions_perf() {
	}
	int test_function() {
		file_system fs;
		fs.set_root(u"F:\\programs\\V2\\interface");
		const auto gui_files = fs.get_root().list_files(u".gui");

		ui::name_maps nmaps;
		ui::definitions defs;
		std::vector<std::pair<std::string, ui::errors>> errors_generated;

		load_ui_definitions_from_directory(
			fs.get_root(), nmaps, defs, errors_generated,
			fake_text_handle_lookup(),
			fake_font_handle_lookup(),
			fake_gobj_lookup(),
			fake_sound_lookup());

		return (int)errors_generated.size();
	}
};

int main() {
	logging_object log;
	{
		test_object<50, 5, gui_definitions_perf> sp_to;
		std::cout << sp_to.log_function(log, "full load of gui object definitions") << std::endl;
	}
    return 0;
}

