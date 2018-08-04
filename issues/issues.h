#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"


namespace scenario {
	class scenario_manager;
}
namespace events {
	struct event_creation_manager;
}

namespace issues {
	struct rules {
		uint32_t rules = 0ui32;

		static constexpr uint32_t build_factory = 0x00000001;
		static constexpr uint32_t expand_factory = 0x00000002;
		static constexpr uint32_t open_factory = 0x00000004;
		static constexpr uint32_t destroy_factory = 0x00000008;
		static constexpr uint32_t factory_priority = 0x00000010;
		static constexpr uint32_t can_subsidise = 0x00000020;
		static constexpr uint32_t pop_build_factory = 0x00000040;
		static constexpr uint32_t pop_expand_factory = 0x00000080;
		static constexpr uint32_t pop_open_factory = 0x00000100;
		static constexpr uint32_t delete_factory_if_no_input = 0x00000200;
		static constexpr uint32_t build_factory_invest = 0x00000400;
		static constexpr uint32_t expand_factory_invest = 0x00000800;
		static constexpr uint32_t open_factory_invest = 0x00001000;
		static constexpr uint32_t build_railway_invest = 0x00002000;
		static constexpr uint32_t can_invest_in_pop_projects = 0x00004000;
		static constexpr uint32_t pop_build_factory_invest = 0x00008000;
		static constexpr uint32_t pop_expand_factory_invest = 0x00010000;
		static constexpr uint32_t pop_open_factory_invest = 0x00020000;
		static constexpr uint32_t allow_foreign_investment = 0x00040000;
		static constexpr uint32_t slavery_allowed = 0x00080000;
		static constexpr uint32_t build_railway = 0x04000000;

		static constexpr uint32_t citizens_rights_mask = 0x00300000;

		static constexpr uint32_t primary_culture_voting = 0x00000000; //citizen's rights
		static constexpr uint32_t culture_voting = 0x00100000; //citizen's rights
		static constexpr uint32_t all_voting = 0x00200000; //citizen's rights

		static constexpr uint32_t voting_system_mask = 0x00C00000;

		static constexpr uint32_t largest_share = 0x00000000; //voting system
		static constexpr uint32_t dhont = 0x00400000; //voting system
		static constexpr uint32_t sainte_laque = 0x00800000; //voting system

		static constexpr uint32_t upper_house_composition_mask = 0x03000000;

		static constexpr uint32_t same_as_ruling_party = 0x00000000; //upper house composition
		static constexpr uint32_t rich_only = 0x01000000; //upper house composition
		static constexpr uint32_t state_vote = 0x02000000; //upper house composition
		static constexpr uint32_t population_vote = 0x03000000; //upper house composition
	};
	struct rules_set {
		rules rules_settings;
		rules rules_mask;
	};
	enum class issue_group : uint8_t {
		party = 0ui8, //note: order matters
		social = 1ui8,
		political = 2ui8,
		economic = 3ui8,
		military = 4ui8
	};
	inline constexpr bool is_unciv_issue(issue_group type) {
		return type == issue_group::economic || type == issue_group::military;
	}
	struct issue_option {
		rules_set issue_rules;
		int32_t technology_cost = 0;
		float war_exhaustion_effect = 1.0f;
		int16_t administrative_multiplier = 0;

		text_data::text_tag name;
		triggers::trigger_tag allow;
		modifiers::national_modifier_tag modifier;

		triggers::trigger_tag on_execute_trigger;
		triggers::effect_tag on_execute_effect;

		issue_tag parent_issue;
		option_tag id;
	};
	struct issue {
		option_tag options[6];

		text_data::text_tag name;
		issue_group type;
		bool next_step_only;
		bool administrative;
		
		issue_tag id;
	};

	class issues_manager {
	public:
		boost::container::flat_map<text_data::text_tag, issue_tag> named_issue_index;
		boost::container::flat_map<text_data::text_tag, option_tag> named_option_index;

		tagged_vector<issue, issue_tag> issues_container;
		tagged_vector<issue_option, option_tag> options;
		std::vector<issue_tag> party_issues;
		std::vector<issue_tag> political_issues;
		std::vector<issue_tag> social_issues;
		std::vector<issue_tag> economic_issues;
		std::vector<issue_tag> military_issues;

		uint32_t options_count = 0ui32;

		uint32_t party_issues_options_count = 0ui32;
		uint32_t political_issues_options_count = 0ui32;
		uint32_t social_issues_options_count = 0ui32;

		uint32_t tracked_options_count = 0ui32; // how many options are tracked in demographics arrays

		option_tag jingoism;
	};
}
