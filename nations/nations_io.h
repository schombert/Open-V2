#pragma once
#include "nations.h"
#include "simple_fs\\simple_fs.h"

class world_state;

namespace nations {
	void read_nation_file(world_state& ws, cultures::national_tag nat_tag_id, date_tag target_date, file const& fi, directory const& units_dir, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_nations_files(world_state& ws, date_tag target_date, directory const& root, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_diplomacy_files(world_state& ws, date_tag target_date, directory const& root);
}
