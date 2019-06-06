#pragma once
#include "common/common.h"
#include "common/shared_tags.h"

class world_state;

namespace map_mode {
	enum class type {
		undefined,
		political,
		prices,
		distance,
		purchasing,
		production,
		population,
		migration,
		relations,
		culture,
		infrastructure,
		military,
		sphere,
		voting,
		revolt_risk,
		region,
		rgo,
		admin
	};

	class legend_gui;

	class state {
	public:
		std::unique_ptr<legend_gui> legends;

		type mode = type::undefined;
		std::atomic<bool> changed = true;

		void init(world_state& ws);

		state();
		~state();
	};

	void change_mode(world_state& ws, type new_mode);
	void on_mouse_over(world_state& ws, provinces::province_tag p);
	void on_selection(world_state& ws, provinces::province_tag p);
	void update_map_colors(world_state& ws);
}
