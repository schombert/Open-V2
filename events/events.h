#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "Parsers\\parsers.h"
#include "simple_fs\\simple_fs.h"
#include "graphics\\texture.h"

namespace scenario {
	class scenario_manager;
}

class world_state;

namespace events {

	struct event_option {
		text_data::text_tag name;
		modifiers::factor_tag ai_chance;
		triggers::effect_tag effect;
	};

	struct event {
		constexpr static uint8_t fire_only_once = 0x0001;
		constexpr static uint8_t is_major = 0x0002;

		event_option options[6];

		text_data::text_tag title;
		text_data::text_tag body;
		graphics::texture_tag picture;

		modifiers::factor_tag mean_time_to_happen;
		triggers::trigger_tag trigger;
		triggers::effect_tag immediate_effect;

		event_tag id;

		uint8_t flags = 0;
	};

	struct decision {
		text_data::text_tag title;
		text_data::text_tag body;
		graphics::texture_tag picture;

		triggers::effect_tag effect;
		triggers::trigger_tag potential;
		triggers::trigger_tag allow;
		modifiers::factor_tag ai_will_do;

		decision_tag id;

		bool no_alert = false;
	};

	class event_manager {
	public:
		tagged_vector<event, event_tag> event_container;
		tagged_vector<decision, decision_tag> decision_container;

		std::vector<event_tag> country_events;
		std::vector<event_tag> province_events;

		std::vector<std::pair<event_tag, uint16_t>> on_yearly_pulse;
		std::vector<std::pair<event_tag, uint16_t>> on_quarterly_pulse;
		std::vector<std::pair<event_tag, uint16_t>> on_new_great_nation;
		std::vector<std::pair<event_tag, uint16_t>> on_lost_great_nation;
		std::vector<std::tuple<event_tag, uint16_t, text_data::text_tag>> on_election_tick; // event + chance + issue group
		std::vector<std::pair<event_tag, uint16_t>> on_colony_to_state; // passes state
		std::vector<std::pair<event_tag, uint16_t>> on_state_conquest; // passes state
		std::vector<std::pair<event_tag, uint16_t>> on_colony_to_state_free_slaves; // passes state
		std::vector<std::pair<event_tag, uint16_t>> on_battle_won; // passes province
		std::vector<std::pair<event_tag, uint16_t>> on_battle_lost; // passes province
		std::vector<std::pair<event_tag, uint16_t>> on_debtor_default; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_debtor_default_small; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_debtor_default_second; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_civilize;
		std::vector<std::pair<event_tag, uint16_t>> on_my_factories_nationalized; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_crisis_declare_interest;


		boost::container::flat_map<int32_t, event_tag> events_by_id;
		boost::container::flat_map<text_data::text_tag, decision_tag> decisions_by_title_index;
	};

	class event_state {
	public:
		tagged_vector<bitfield_type, int32_t> country_event_has_fired;
		tagged_vector<bitfield_type, int32_t> province_event_has_fired;
	};

	using event_slot_content = std::variant<std::monostate, nations::country_tag, provinces::province_tag, population::pop_tag, nations::state_tag>;

	void fire_event(world_state& ws, event_tag e, event_slot_content primary_contents, event_slot_content from);
	void fire_delayed_event(world_state& ws, event_tag e, int32_t delay_days, event_slot_content primary_contents, event_slot_content from);
	void fire_event_from_list(world_state& ws, std::vector<std::pair<event_tag, uint16_t>> const& event_list, event_slot_content primary_contents, event_slot_content from);

	void execute_decision_set(std::vector<std::pair<nations::country_tag, events::decision_tag>>const& decision_set, world_state& ws);
}
