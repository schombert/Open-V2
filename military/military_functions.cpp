#include "common\\common.h"
#include "military_functions.h"
#include "world_state\\world_state.h"
#include <random>

namespace military {
	military_leader& make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general) {
		military_leader& new_leader = ws.w.military_s.leaders.get_new();
		auto& culture_obj = ws.s.culture_m.culture_container[culture];
		auto& cgroup = ws.s.culture_m.culture_groups[culture_obj.group];
		if(is_general) {
			std::uniform_int_distribution<int32_t> r(0, int32_t(cgroup.leader_pictures.general_size) - 1);
			new_leader.portrait = ws.s.culture_m.leader_pictures[size_t(cgroup.leader_pictures.general_offset + r(get_local_generator()))];
		} else {
			std::uniform_int_distribution<int32_t> r(0, int32_t(cgroup.leader_pictures.admiral_size) - 1);
			new_leader.portrait = ws.s.culture_m.leader_pictures[size_t(cgroup.leader_pictures.admiral_offset + r(get_local_generator()))];
		}
		return new_leader;
	}

	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date) {
		military_leader& new_leader = make_empty_leader(ws, culture, is_general);
		new_leader.creation_date = creation_date;

		auto first_name_range = ws.s.culture_m.first_names_by_culture.get_row(to_index(culture));
		auto last_name_range = ws.s.culture_m.first_names_by_culture.get_row(to_index(culture));

		std::uniform_int_distribution<int32_t> fn(0, int32_t(first_name_range.second - first_name_range.first) - 1);
		std::uniform_int_distribution<int32_t> ln(0, int32_t(last_name_range.second - last_name_range.first) - 1);
		std::uniform_int_distribution<int32_t> br(0, int32_t(ws.s.military_m.background_traits.size()) - 1);
		std::uniform_int_distribution<int32_t> pr(0, int32_t(ws.s.military_m.personality_traits.size()) - 1);

		auto& local_generator = get_local_generator();

		new_leader.first_name = *(first_name_range.first + fn(local_generator));
		new_leader.last_name = *(last_name_range.first + ln(local_generator));
		new_leader.background = ws.s.military_m.background_traits[size_t(br(local_generator))];
		new_leader.personality = ws.s.military_m.personality_traits[size_t(pr(local_generator))];

		calculate_leader_traits(ws, new_leader);

		return new_leader.id;
	}

	void calculate_leader_traits(world_state& ws, military_leader& l) {
		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>> dest_vec(l.leader_traits);

		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>, Eigen::Aligned32> source_a(ws.s.military_m.leader_trait_definitions.get_row(l.background));
		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>, Eigen::Aligned32> source_b(ws.s.military_m.leader_trait_definitions.get_row(l.personality));

		dest_vec = source_a + source_b;
	}
}
