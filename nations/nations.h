#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "economy\\economy.h"

namespace nations {
	struct state_instance;

	struct region_state_pair {
		provinces::state_tag region_id;
		state_instance* state;

		bool operator<(region_state_pair other)  const noexcept { return region_id < other.region_id; }
		bool operator==(region_state_pair other) const noexcept { return region_id == other.region_id; }
	};

	struct nation {
		atomic_tag<date_tag> last_update;

		set_tag<provinces::province_tag> owned_provinces;
		set_tag<provinces::province_tag> controlled_provinces;
		set_tag<region_state_pair> member_states;

		text_data::text_tag name;
		text_data::text_tag adjective;

		graphics::texture_tag flag;

		graphics::color_rgb current_color;
		country_tag id;

		cultures::national_tag tag;

		cultures::culture_tag dominant_culture;
		cultures::religion_tag dominant_religion;
	};

	struct state_instance {
		economy::factory_instance factories[8] = {};
		nation* owner;
		atomic_tag<date_tag> last_update;

		cultures::culture_tag dominant_culture;

		state_tag id;
		provinces::state_tag region_id;

		cultures::religion_tag dominant_religion;
		uint8_t flags = 0ui8;

		constexpr static uint8_t is_slave_state = 0x01;
		constexpr static uint8_t is_colonial = 0x02;
		constexpr static uint8_t is_protectorate = 0x02;
	};

	class nations_state {
	public:
		stable_vector<nation, country_tag, 512, 16> nations;
		stable_vector<state_instance, state_tag, 512, 16> states;

		stable_2d_vector<governments::party_tag, country_tag, ideologies::ideology_tag, 512, 16> active_parties;
		stable_variable_vector_storage_mk_2<region_state_pair, 2, 8192> state_arrays;

		stable_2d_vector<int32_t, state_tag, population::demo_tag, 512, 16> state_demographics;
		stable_2d_vector<int64_t, country_tag, population::demo_tag, 512, 16> nation_demographics;
	};
}
