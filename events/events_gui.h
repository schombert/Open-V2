#pragma once
#include "common\\common.h"
#include "events.h"
#include "gui\\gui.h"

class world_state;

namespace events {
	class province_event_window_t;
	class nation_event_window_t;
	class major_event_window_t;

	using from_variant = std::variant<std::monostate, provinces::province_tag, nations::country_tag, nations::state_tag, population::pop_tag, population::rebel_faction_tag>;

	struct pending_province_event {
		event_tag e;
		provinces::province_tag province_for;
		from_variant event_from;
		jsf_prng generator;
	};

	struct pending_nation_event {
		event_tag e;
		nations::country_tag nation_for;
		from_variant event_from;
		jsf_prng generator;
	};

	class province_event_window {
	public:
		std::unique_ptr<province_event_window_t> win;
		pending_province_event displayed_event;

		province_event_window();
		~province_event_window();

		void hide_province_event_window(ui::gui_manager& gui_m);
		void show_province_event_window(ui::gui_manager& gui_m, pending_province_event e);
		void init_province_event_window(world_state& ws);
	};

	class nation_event_window {
	public:
		std::unique_ptr<nation_event_window_t> win;
		pending_nation_event displayed_event;

		nation_event_window();
		~nation_event_window();

		void hide_nation_event_window(ui::gui_manager& gui_m);
		void show_nation_event_window(ui::gui_manager& gui_m, pending_nation_event e);
		void init_nation_event_window(world_state& ws);
	};

	class major_event_window {
	public:
		std::unique_ptr<major_event_window_t> win;
		pending_nation_event displayed_event;

		major_event_window();
		~major_event_window();

		void hide_major_event_window(ui::gui_manager& gui_m);
		void show_major_event_window(ui::gui_manager& gui_m, pending_nation_event e);
		void init_major_event_window(world_state& ws);
	};
}
