#include "trigger_reading.h"
#include "codes.h"
#include "scenario\\scenario.h"
#include <optional>
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"
#include <variant>
#include <algorithm>

#ifdef _DEBUG
#define TRIGGER_ERROR(type, environment) throw type ();
#else
#define TRIGGER_ERROR(type, environment) std::abort();
#endif

namespace triggers {
	using trigger_value = std::variant<std::monostate, int32_t, float, issues::option_identifier, trigger_payload>;

	struct year_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return uint16_t(trigger_codes::year | association_to_trigger_code(a));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct is_canal_enabled_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return uint16_t(trigger_codes::is_canal_enabled | association_to_bool_code(a));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct month_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return uint16_t(trigger_codes::month | association_to_trigger_code(a));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct great_wars_enabled_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return uint16_t(trigger_codes::great_wars_enabled | association_to_bool_code(a, t));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<bool>(t) );
		}
	};
	struct world_wars_enabled_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return uint16_t(trigger_codes::world_wars_enabled | association_to_bool_code(a, t));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct crisis_exist_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return uint16_t(trigger_codes::crisis_exist | association_to_bool_code(a, t));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_liberation_crisis_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return uint16_t(trigger_codes::is_liberation_crisis | association_to_bool_code(a, t));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_claim_crisis_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return uint16_t(trigger_codes::is_claim_crisis | association_to_bool_code(a, t));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};

	struct port_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::port | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct involved_in_crisis_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::involved_in_crisis_pop | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::involved_in_crisis_nation | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_cultural_sphere_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::has_cultural_sphere | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct social_movement_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::social_movement | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct political_movement_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::political_movement | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct rich_tax_above_poor_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::rich_tax_above_poor | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_substate_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::is_substate | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_flashpoint_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::has_flashpoint | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_disarmed_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::is_disarmed | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct can_nationalize_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::can_nationalize | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct part_of_sphere_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::part_of_sphere | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct constructing_cb_discovered_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::constructing_cb_discovered | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct colonial_nation_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::colonial_nation | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_capital_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::is_capital | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct election_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::election | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct always_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return uint16_t(trigger_codes::always | association_to_bool_code(a, t));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_releasable_vassal_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation)
					return uint16_t(trigger_codes::is_releasable_vassal_from | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return uint16_t(trigger_codes::is_releasable_vassal_other | association_to_bool_code(a, t));
				else
					return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct someone_can_form_union_tag_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation)
					return uint16_t(trigger_codes::someone_can_form_union_tag_from | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return uint16_t(trigger_codes::someone_can_form_union_tag_other | association_to_bool_code(a, t));
				else
					return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_state_capital_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::is_state_capital | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_factories_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::has_factories | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_empty_adjacent_province_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::has_empty_adjacent_province | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct ai_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::ai | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct minorities_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::minorities_nation | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::minorities_state | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::minorities_province | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct culture_has_union_tag_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::culture_has_union_tag_nation | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::culture_has_union_tag_pop | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_colonial_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::is_colonial_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::is_colonial_state | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_greater_power_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::is_greater_power_pop | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::is_greater_power_nation | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct can_create_vassals_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::can_create_vassals | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_recently_lost_war_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::has_recently_lost_war | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_mobilised_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::is_mobilised | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct crime_higher_than_education_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::crime_higher_than_education_nation | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::crime_higher_than_education_state | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::crime_higher_than_education_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::crime_higher_than_education_pop | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct civilized_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::civilized_nation | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::civilized_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::civilized_pop | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};

	struct rank_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::rank | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct crisis_temperature_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return uint16_t(trigger_codes::crisis_temperature | association_to_trigger_code(a));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct has_recent_imigration_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::has_recent_imigration | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct province_control_days_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::province_control_days | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct num_of_substates_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::num_of_substates | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct num_of_vassals_no_substates_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::num_of_vassals_no_substates | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct number_of_states_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::number_of_states | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct war_score_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::war_score | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct flashpoint_tension_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::flashpoint_tension | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct life_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::war_score | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct everyday_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::everyday_needs | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct luxury_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::luxury_needs | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct social_movement_strength_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::social_movement_strength | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct political_movement_strength_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::political_movement_strength | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct total_num_of_ports_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::total_num_of_ports | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct agree_with_ruling_party_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::agree_with_ruling_party | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct constructing_cb_progress_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::constructing_cb_progress | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct civilization_progress_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::civilization_progress | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct rich_strata_life_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::rich_strata_life_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::rich_strata_life_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::rich_strata_life_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::rich_strata_life_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct rich_strata_everyday_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::rich_strata_everyday_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::rich_strata_everyday_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::rich_strata_everyday_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::rich_strata_everyday_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct rich_strata_luxury_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::rich_strata_luxury_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::rich_strata_luxury_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::rich_strata_luxury_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::rich_strata_luxury_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	
	struct middle_strata_life_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::middle_strata_life_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::middle_strata_life_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::middle_strata_life_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::middle_strata_life_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct middle_strata_everyday_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::middle_strata_everyday_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::middle_strata_everyday_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::middle_strata_everyday_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::middle_strata_everyday_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct middle_strata_luxury_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::middle_strata_luxury_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::middle_strata_luxury_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::middle_strata_luxury_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::middle_strata_luxury_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};

	struct poor_strata_life_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::poor_strata_life_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::poor_strata_life_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::poor_strata_life_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::poor_strata_life_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct poor_strata_everyday_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::poor_strata_everyday_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::poor_strata_everyday_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::poor_strata_everyday_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::poor_strata_everyday_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct poor_strata_luxury_needs_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::poor_strata_luxury_needs_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::poor_strata_luxury_needs_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::poor_strata_luxury_needs_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::poor_strata_luxury_needs_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};

	struct revanchism_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::revanchism_pop | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::revanchism_nation | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct poor_strata_militancy_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::poor_strata_militancy_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::poor_strata_militancy_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::poor_strata_militancy_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::poor_strata_militancy_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct middle_strata_militancy_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::middle_strata_militancy_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::middle_strata_militancy_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::middle_strata_militancy_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::middle_strata_militancy_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct rich_strata_militancy_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::rich_strata_militancy_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::rich_strata_militancy_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::rich_strata_militancy_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::rich_strata_militancy_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct consciousness_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::consciousness_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::consciousness_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::consciousness_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::consciousness_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct literacy_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::literacy_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::literacy_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::literacy_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::literacy_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct militancy_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::militancy_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::militancy_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::militancy_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::militancy_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct military_spending_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::military_spending_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::military_spending_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::military_spending_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::military_spending_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct administration_spending_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::administration_spending_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::administration_spending_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::administration_spending_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::administration_spending_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct education_spending_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::education_spending_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::education_spending_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::education_spending_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::education_spending_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct national_provinces_occupied_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::national_provinces_occupied | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct social_spending_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::social_spending_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::social_spending_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::social_spending_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct brigades_compare_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (scope.this_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::brigades_compare_this | association_to_trigger_code(a));
				else if (scope.from_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::brigades_compare_from | association_to_trigger_code(a));
				else
					return std::optional<uint16_t>();
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct rich_tax_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::rich_tax | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct middle_tax_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::middle_tax | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct poor_tax_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::poor_tax | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct mobilisation_size_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::mobilisation_size | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct province_id_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::province_id | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct invention_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::invention | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.technology_m.named_invention_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct big_producer_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::big_producer | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct strata_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop) {
				if (is_fixed_token_ci(t.start, t.end, "poor"))
					return uint16_t(trigger_codes::strata_poor | association_to_bool_code(a));
				else if (is_fixed_token_ci(t.start, t.end, "rich"))
					return uint16_t(trigger_codes::strata_rich | association_to_bool_code(a));
				else
					return  uint16_t(trigger_codes::strata_middle | association_to_bool_code(a));
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type&, const scenario::scenario_manager&) {
			return trigger_value();
		}
	};
	struct life_rating_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::life_rating_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::life_rating_state | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct has_empty_adjacent_state_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::has_empty_adjacent_state_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::has_empty_adjacent_state_state | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct state_id_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::state_id_province | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::state_id_state | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct cash_reserves_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::cash_reserves | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct unemployment_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::unemployment_pop | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::unemployment_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::unemployment_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::unemployment_nation | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct is_slave_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::is_slave_pop | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::is_slave_state | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::is_slave_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::is_slave_nation | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_independant_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::is_independant | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_national_minority_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::has_national_minority_state | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::has_national_minority_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::has_national_minority_nation | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct government_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::government_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::government_pop | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.governments_m.named_government_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct constructing_cb_type_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::constructing_cb_type | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.military_m.named_cb_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct can_build_factory_in_capital_state_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::can_build_factory_in_capital_state | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_factory_types_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct capital_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::capital | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct tech_school_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::tech_school | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.technology_m.named_tech_school_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct primary_culture_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::primary_culture | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_crime_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::has_crime | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct accepted_culture_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::accepted_culture | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_religion_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::pop_majority_religion_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::pop_majority_religion_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::pop_majority_religion_province | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_culture_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::pop_majority_culture_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::pop_majority_culture_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::pop_majority_culture_province | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_issue_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::pop_majority_issue_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::pop_majority_issue_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::pop_majority_issue_province | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::pop_majority_issue_pop | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_value(
				tag_from_text(
					s.issues_m.named_option_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_ideology_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::pop_majority_ideology_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::pop_majority_ideology_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::pop_majority_ideology_province | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::pop_majority_ideology_pop | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct trade_goods_in_state_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::trade_goods_in_state_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::trade_goods_in_state_province | association_to_bool_code(a));

			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct culture_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::culture_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::culture_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::culture_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::culture_this_province | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return std::optional<uint16_t>();
				}
			} else if (is_fixed_token_ci(t, "from")) {
				if (scope.main_slot == trigger_slot_contents::pop && scope.from_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::culture_from_nation | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			} else if (is_fixed_token_ci(t, "reb")) {
				if (scope.contains_rebeltype == false)
					return std::optional<uint16_t>();
				else if (scope.main_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::culture_pop_reb | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::state)
					return  uint16_t(trigger_codes::culture_state_reb | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::province)
					return  uint16_t(trigger_codes::culture_province_reb | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::culture_nation_reb | association_to_bool_code(a));
			}
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::culture_pop | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::culture_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::culture_province | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::culture_nation | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_culture_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.this_slot == trigger_slot_contents::pop) {
					if (scope.main_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::has_pop_culture_nation_this_pop | association_to_bool_code(a));
					else if (scope.main_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::has_pop_culture_pop_this_pop | association_to_bool_code(a));
					else if (scope.main_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::has_pop_culture_state_this_pop | association_to_bool_code(a));
					else if (scope.main_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::has_pop_culture_province_this_pop | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return std::optional<uint16_t>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::has_pop_culture_pop | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::state)
					return  uint16_t(trigger_codes::has_pop_culture_state | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::province)
					return  uint16_t(trigger_codes::has_pop_culture_province | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::has_pop_culture_nation | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_religion_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.this_slot == trigger_slot_contents::pop) {
					if (scope.main_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::has_pop_religion_nation_this_pop | association_to_bool_code(a));
					else if (scope.main_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::has_pop_religion_pop_this_pop | association_to_bool_code(a));
					else if (scope.main_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::has_pop_religion_state_this_pop | association_to_bool_code(a));
					else if (scope.main_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::has_pop_religion_province_this_pop | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return std::optional<uint16_t>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::has_pop_religion_pop | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::state)
					return  uint16_t(trigger_codes::has_pop_religion_state | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::province)
					return  uint16_t(trigger_codes::has_pop_religion_province | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::has_pop_religion_nation | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct culture_group_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::culture_group_pop_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::culture_group_pop_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::culture_group_pop_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::culture_group_pop_this_province | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (scope.main_slot == trigger_slot_contents::nation) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::culture_group_nation_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::culture_group_nation_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::culture_group_nation_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::culture_group_nation_this_province | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return std::optional<uint16_t>();
				}
			} else if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation) {
					if (scope.main_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::culture_group_pop_from_nation | association_to_bool_code(a));
					else if (scope.main_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::culture_group_nation_from_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return std::optional<uint16_t>();
				}
			} else if (is_fixed_token_ci(t, "reb")) {
				if (scope.contains_rebeltype == false)
					return std::optional<uint16_t>();
				else if (scope.main_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::culture_group_reb_pop | association_to_bool_code(a));
				else if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::culture_group_reb_nation | association_to_bool_code(a));
			}
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::culture_group_pop | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::culture_group_nation | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct religion_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::religion_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::religion_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::religion_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::religion_this_pop | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::religion_from_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "reb")) {
					return  uint16_t(trigger_codes::religion_reb | association_to_bool_code(a));
				} else {
					return  uint16_t(trigger_codes::religion | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct terrain_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop) {
				return  uint16_t(trigger_codes::terrain_pop | association_to_bool_code(a));
			} else if (scope.main_slot == trigger_slot_contents::province) {
				return  uint16_t(trigger_codes::terrain_province | association_to_bool_code(a));
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct trade_goods_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::trade_goods | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct is_secondary_power_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::is_secondary_power_pop | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::is_secondary_power_nation | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_faction_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::has_faction_pop | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::has_faction_nation | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.population_m.named_rebel_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_unclaimed_cores_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::has_unclaimed_cores | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_cultural_union_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes") || is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::is_cultural_union_bool | association_to_bool_code(a, t));
				else
					return std::optional<uint16_t>();
			} else if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::is_cultural_union_this_self_pop | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::is_cultural_union_this_pop | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::state)
					return  uint16_t(trigger_codes::is_cultural_union_this_state | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::province)
					return  uint16_t(trigger_codes::is_cultural_union_this_province | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::is_cultural_union_this_nation | association_to_bool_code(a));
				else if (scope.contains_rebeltype)
					return  uint16_t(trigger_codes::is_cultural_union_this_rebel | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::is_cultural_union_tag_nation | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::is_cultural_union_tag_this_pop | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::state)
					return  uint16_t(trigger_codes::is_cultural_union_tag_this_state | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::province)
					return  uint16_t(trigger_codes::is_cultural_union_tag_this_province | association_to_bool_code(a));
				else if (scope.this_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::is_cultural_union_tag_this_nation | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct can_build_factory_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::can_build_factory | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct war_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::war_pop | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::war_nation | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct war_exhaustion_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::war_exhaustion | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct blockade_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::blockade | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct owns_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::owns | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct controls_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::controls | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct is_core_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_integer(t.start, t.end)) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::is_core_integer | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			} else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::is_core_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::is_core_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::is_core_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_core_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_core_from_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return  uint16_t(trigger_codes::is_core_reb | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::is_core_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			if (is_integer(t.start, t.end)) {
				return trigger_payload{ token_to<uint16_t>(t) };
			} else {
				return trigger_payload(
					tag_from_text(
						s.culutre_m.national_tags_index,
						cultures::tag_to_encoding(t.start, t.end)));
			}
		}
	};
	struct num_of_revolts_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::num_of_revolts | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct revolt_percentage_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::revolt_percentage | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct num_of_cities_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::num_of_cities_this_nation | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::num_of_cities_this_province | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::num_of_cities_this_state | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::num_of_cities_this_pop | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::num_of_cities_from_nation | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::num_of_cities_int | association_to_trigger_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct num_of_ports_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::num_of_ports | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct num_of_allies_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::num_of_allies | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct num_of_vassals_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::num_of_vassals | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct owned_by_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::owned_by_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::owned_by_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::owned_by_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::owned_by_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::owned_by_from_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::owned_by_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct exists_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes") | is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return  uint16_t(trigger_codes::exists_bool | association_to_bool_code(a, t));
				else
					return std::optional<uint16_t>();
			} else {
				return  uint16_t(trigger_codes::exists_tag | association_to_bool_code(a));
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct has_country_flag_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::has_country_flag | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_payload{
				s.variables_m.get_named_national_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct has_global_flag_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return  uint16_t(trigger_codes::has_global_flag | association_to_bool_code(a));
		}
		static trigger_value read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_payload{
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct continent_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_nation_this | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_nation_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::continent_nation | association_to_bool_code(a));
				}
			} else if (scope.main_slot == trigger_slot_contents::state) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_state_this | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_state_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::continent_state | association_to_bool_code(a));
				}
			} else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_province_this | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_province_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::continent_province | association_to_bool_code(a));
				}
			} else if (scope.main_slot == trigger_slot_contents::pop) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_pop_this | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::continent_pop_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::continent_pop | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct casus_belli_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::casus_belli_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::casus_belli_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::casus_belli_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::casus_belli_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::casus_belli_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::casus_belli_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct military_access_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::military_access_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::military_access_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::military_access_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::military_access_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::military_access_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::military_access_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct prestige_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::prestige_this_pop | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::prestige_this_state | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::prestige_this_province | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::prestige_this_nation | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::prestige_from | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::prestige_value | association_to_trigger_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct badboy_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::badboy | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct has_building_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::has_building_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "fort"))
					return  uint16_t(trigger_codes::has_building_fort | association_to_bool_code(a));
				else if (is_fixed_token_ci(t, "railroad"))
					return  uint16_t(trigger_codes::has_building_railroad | association_to_bool_code(a));
				else if (is_fixed_token_ci(t, "naval_base"))
					return  uint16_t(trigger_codes::has_building_naval_base | association_to_bool_code(a));
				else
					return std::optional<uint16_t>();
			} else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_factory_types_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct empty_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::empty | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_blockaded_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::is_blockaded | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct has_country_modifier_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::has_country_modifier | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_province_modifier_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::has_province_modifier | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct nationalvalue_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::nationalvalue | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct region_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::region | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.province_m.named_states_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct tag_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::tag_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::tag_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::tag_from_nation | association_to_bool_code(a));
					else if (scope.from_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::tag_from_province | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::tag_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct neighbour_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::neighbour_this | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::neighbour_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::neighbour_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct units_in_province_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::units_in_province_this_pop | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::units_in_province_this_state | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::units_in_province_this_province | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::units_in_province_this_nation | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::units_in_province_from | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::units_in_province_value | association_to_trigger_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct war_with_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::war_with_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::war_with_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::war_with_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::war_with_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::war_with_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::war_with_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct unit_in_battle_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::unit_in_battle | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct total_amount_of_divisions_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::total_amount_of_divisions | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct money_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::money | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct lost_national_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::lost_national | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct is_vassal_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::is_vassal | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct ruling_party_ideology_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::ruling_party_ideology_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::ruling_party_ideology_pop | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct ruling_party_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::ruling_party | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct has_leader_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::has_leader | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct is_ideology_enabled_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return  uint16_t(trigger_codes::is_ideology_enabled | association_to_bool_code(a));
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct political_reform_want_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::political_reform_want_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::political_reform_want_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct social_reform_want_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::social_reform_want_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::social_reform_want_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct total_amount_of_ships_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::total_amount_of_ships | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<uint16_t>(t) };
		}
	};
	struct plurality_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::plurality | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct corruption_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::corruption | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value{ token_to<float>(t) };
		}
	};
	struct is_state_religion_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::is_state_religion_state | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::is_state_religion_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::is_state_religion_pop | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};

	struct is_primary_culture_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::nation) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_primary_culture_nation_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::is_primary_culture_nation_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::is_primary_culture_nation_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::is_primary_culture_nation_this_pop | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (scope.main_slot == trigger_slot_contents::state) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_primary_culture_state_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::is_primary_culture_state_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::is_primary_culture_state_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::is_primary_culture_state_this_pop | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (scope.main_slot == trigger_slot_contents::province) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_primary_culture_province_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::is_primary_culture_province_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::is_primary_culture_province_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::is_primary_culture_province_this_pop | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_primary_culture_pop_this_nation | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::is_primary_culture_pop_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::is_primary_culture_pop_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::is_primary_culture_pop_this_pop | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return std::optional<uint16_t>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::state)
					return  uint16_t(trigger_codes::is_primary_culture_state | association_to_bool_code(a, t));
				else if (scope.main_slot == trigger_slot_contents::province)
					return  uint16_t(trigger_codes::is_primary_culture_province | association_to_bool_code(a, t));
				else if (scope.main_slot == trigger_slot_contents::pop)
					return  uint16_t(trigger_codes::is_primary_culture_pop | association_to_bool_code(a, t));
				else
					return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_accepted_culture_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::is_accepted_culture_state | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::is_accepted_culture_province | association_to_bool_code(a, t));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::is_accepted_culture_pop | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();

		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct is_coastal_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::is_coastal | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct in_sphere_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::in_sphere_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::in_sphere_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::in_sphere_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::in_sphere_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::in_sphere_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::in_sphere_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct produces_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::produces_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::produces_province | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::produces_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::produces_pop | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_type_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::has_pop_type_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::has_pop_type_province | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::has_pop_type_state | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::has_pop_type_pop | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.population_m.named_pop_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct total_pops_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::total_pops_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::total_pops_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::total_pops_state | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::total_pops_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<int32_t>(t));
		}
	};
	struct average_militancy_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::average_militancy_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::average_militancy_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::average_militancy_state | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct average_consciousness_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::average_consciousness_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::average_consciousness_province | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::state)
				return  uint16_t(trigger_codes::average_consciousness_state | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct is_next_reform_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::is_next_reform_nation | association_to_bool_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::is_next_reform_pop | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_value(
				tag_from_text(
					s.issues_m.named_option_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct rebel_power_fraction_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::rebel_power_fraction | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct recruited_percentage_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::recruited_percentage_nation | association_to_trigger_code(a));
			else if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::recruited_percentage_pop | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct has_culture_core_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return  uint16_t(trigger_codes::has_culture_core | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct nationalism_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::nationalism | association_to_trigger_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct is_overseas_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::is_overseas | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct controlled_by_rebels_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return  uint16_t(trigger_codes::controlled_by_rebels | association_to_bool_code(a, t));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload{ token_to<bool>(t) };
		}
	};
	struct controlled_by_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::controlled_by_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::controlled_by_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::controlled_by_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::controlled_by_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::controlled_by_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "owner")) {
					return  uint16_t(trigger_codes::controlled_by_owner | association_to_bool_code(a));
				} else if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return  uint16_t(trigger_codes::controlled_by_reb | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::controlled_by_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct truce_with_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::truce_with_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::truce_with_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::truce_with_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::truce_with_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::truce_with_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::truce_with_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct is_sphere_leader_of_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::is_sphere_leader_of_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::is_sphere_leader_of_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::is_sphere_leader_of_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_sphere_leader_of_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_sphere_leader_of_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::is_sphere_leader_of_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct constructing_cb_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::constructing_cb_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::constructing_cb_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::constructing_cb_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::constructing_cb_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::constructing_cb_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::constructing_cb_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct vassal_of_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::vassal_of_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::vassal_of_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::vassal_of_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::vassal_of_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::vassal_of_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::vassal_of_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct substate_of_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::substate_of_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::substate_of_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::substate_of_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::substate_of_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::substate_of_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::substate_of_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct is_our_vassal_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::is_our_vassal_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::is_our_vassal_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::is_our_vassal_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_our_vassal_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::is_our_vassal_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::is_our_vassal_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct this_culture_union_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::this_culture_union_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::this_culture_union_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::this_culture_union_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::this_culture_union_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "this_union")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::this_culture_union_this_union_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::this_culture_union_this_union_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::this_culture_union_this_union_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::this_culture_union_this_union_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::this_culture_union_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::this_culture_union_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct alliance_with_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::alliance_with_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::alliance_with_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::alliance_with_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::alliance_with_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::alliance_with_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::alliance_with_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct in_default_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::in_default_this_pop | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::in_default_this_state | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::in_default_this_province | association_to_bool_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::in_default_this_nation | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::in_default_from | association_to_bool_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::in_default_tag | association_to_bool_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct industrial_score_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::industrial_score_this_pop | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::industrial_score_this_state | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::industrial_score_this_province | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::industrial_score_this_nation | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::industrial_score_from_nation | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::industrial_score_value | association_to_trigger_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct military_score_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return  uint16_t(trigger_codes::military_score_this_pop | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::state)
						return  uint16_t(trigger_codes::military_score_this_state | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::province)
						return  uint16_t(trigger_codes::military_score_this_province | association_to_trigger_code(a));
					else if (scope.this_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::military_score_this_nation | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return  uint16_t(trigger_codes::military_score_from_nation | association_to_trigger_code(a));
					else
						return std::optional<uint16_t>();
				} else {
					return  uint16_t(trigger_codes::military_score_value | association_to_trigger_code(a));
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_value(token_to<float>(t));
		}
	};
	struct is_possible_vassal_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return  uint16_t(trigger_codes::is_possible_vassal | association_to_bool_code(a));
			else
				return std::optional<uint16_t>();
		}
		static trigger_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};


	//for scope triggers
	struct or_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return uint16_t(trigger_codes::is_disjunctive_scope | trigger_codes::generic_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct and_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return trigger_codes::generic_scope;
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct not_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return trigger_codes::placeholder_not_scope;
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct any_neighbor_province_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::x_neighbor_province_scope | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct any_neighbor_country_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_neighbor_country_scope_nation | trigger_codes::is_existance_scope);
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::x_neighbor_country_scope_pop | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct war_countries_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_war_countries_scope_nation);
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::x_war_countries_scope_pop);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct any_greater_power_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return uint16_t(trigger_codes::x_greater_power_scope | trigger_codes::is_existance_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct any_owned_province_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_owned_province_scope_nation | trigger_codes::is_existance_scope);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::x_owned_province_scope_state | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};

	struct any_core_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_core_scope_nation | trigger_codes::is_existance_scope);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::x_core_scope_province | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				return trigger_scope_state{
					trigger_slot_contents::province,
					scope.this_slot,
					scope.from_slot,
					scope.contains_rebeltype };
			} else {
				return trigger_scope_state{
					trigger_slot_contents::nation,
					scope.this_slot,
					scope.from_slot,
					scope.contains_rebeltype };
			}
		}
	};

	struct all_core_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_core_scope_nation);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::x_core_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				return trigger_scope_state{
					trigger_slot_contents::province,
					scope.this_slot,
					scope.from_slot,
					scope.contains_rebeltype };
			} else {
				return trigger_scope_state{
					trigger_slot_contents::nation,
					scope.this_slot,
					scope.from_slot,
					scope.contains_rebeltype };
			}
		}
	};
	struct any_state_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_state_scope | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::state,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct any_substate_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_substate_scope | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct any_sphere_member_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_sphere_member_scope | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct any_pop_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::x_pop_scope_nation | trigger_codes::is_existance_scope);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::x_pop_scope_state | trigger_codes::is_existance_scope);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::x_pop_scope_province | trigger_codes::is_existance_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::pop,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct owner_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::owner_scope_state);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::owner_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct controller_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::controller_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct location_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::location_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct country_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::country_scope_state);
			else if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::country_scope_pop);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct capital_scope_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::capital_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct this_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.this_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::this_scope_nation);
			else if (scope.this_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::this_scope_province);
			else if (scope.this_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::this_scope_state);
			else if (scope.this_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::this_scope_pop);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				scope.this_slot,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct from_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.from_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::from_scope_nation);
			else if (scope.from_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::from_scope_province);
			else if (scope.from_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::from_scope_state);
			else if (scope.from_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::from_scope_pop);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				scope.from_slot,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct sea_zone_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::sea_zone_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct cultural_union_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::cultural_union_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct overlord_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::overlord_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct sphere_owner_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(trigger_codes::sphere_owner_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct independence_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.contains_rebeltype)
				return uint16_t(trigger_codes::independence_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct flashpoint_tag_scope_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(trigger_codes::flashpoint_tag_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct crisis_state_scope_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return uint16_t(trigger_codes::crisis_state_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::state,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};
	struct state_scope_trigger {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(trigger_codes::state_scope_pop);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(trigger_codes::state_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::state,
				scope.this_slot,
				scope.from_slot,
				scope.contains_rebeltype };
		}
	};

#ifdef _DEBUG
	struct no_code_value_found_for_scope_and_argument {};
	struct no_payload_value {};
	struct mismatched_payload_size {};
	struct unknown_trigger {};
#endif

	struct diplomatic_influence_trigger {
		trigger_parsing_environment& env;

		uint16_t value = 0;
		association_type a = association_type::eq_default;

		cultures::national_tag who;
		bool from_v = false;
		bool this_v = false;

		diplomatic_influence_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_who(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this"))
				this_v = true;
			else if (is_fixed_token_ci(t, "from"))
				from_v = true;
			else
				who = tag_from_text(env.s.culutre_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end));
		}
		void set_value(const std::pair<association_type, token_and_type>& p) {
			a = p.first;
			value = token_to<uint16_t>(p.second);
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
			if (from_v) {
				if(env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::diplomatic_influence_from_nation | association_to_trigger_code(a)));
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::diplomatic_influence_from_province | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else if (this_v) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::diplomatic_influence_this_nation | association_to_trigger_code(a)));
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::diplomatic_influence_this_province | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else {
				env.data.push_back(uint16_t(trigger_codes::diplomatic_influence_tag | association_to_trigger_code(a)));
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(value).value);
				env.data.push_back(trigger_payload(who).value);
			}
		}
	};

	struct pop_unemployment_trigger {
		trigger_parsing_environment& env;

		float value = 0;
		population::pop_type_tag type;
		bool this_v = false;
		association_type a;

		pop_unemployment_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_type_or_pop(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this"))
				this_v = true;
			else
				type = tag_from_text(
					env.s.population_m.named_pop_type_index,
					text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_value(const std::pair<association_type, token_and_type>& p) {
			a = p.first;
			value = token_to<float>(p.second);
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				throw no_code_value_found_for_scope_and_argument();
			if (this_v) {
				if(env.current_scope.this_slot != trigger_slot_contents::pop)
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				if (env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::pop_unemployment_nation_this_pop | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::state)
					env.data.push_back(uint16_t(trigger_codes::pop_unemployment_state_this_pop | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::pop_unemployment_province_this_pop | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				env.data.push_back(3ui16);
				add_float_to_payload(env.data, value);
			} else {
				if (env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::pop_unemployment_nation | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::state)
					env.data.push_back(uint16_t(trigger_codes::pop_unemployment_state | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::pop_unemployment_province | association_to_trigger_code(a)));
				else if(env.current_scope.main_slot == trigger_slot_contents::pop)
					env.data.push_back(uint16_t(trigger_codes::pop_unemployment_pop | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				env.data.push_back(4ui16);
				add_float_to_payload(env.data, value);
				env.data.push_back(trigger_payload(type).value);
			}
		}
	};
	struct relation_trigger {
		trigger_parsing_environment& env;

		int16_t value = 0;
		cultures::national_tag who;
		bool from_v = false;
		bool this_v = false;
		association_type a;

		relation_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_who(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this"))
				this_v = true;
			else if (is_fixed_token_ci(t, "from"))
				from_v = true;
			else
				who = tag_from_text(env.s.culutre_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end));
		}
		void set_value(const std::pair<association_type, token_and_type>& p) {
			a = p.first;
			value = token_to<int16_t>(p.second);
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				throw no_code_value_found_for_scope_and_argument();
			if (from_v) {
				if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::relation_from_nation | association_to_trigger_code(a)));
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::relation_from_province | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else if (this_v) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::relation_this_nation | association_to_trigger_code(a)));
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::relation_this_province | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else {
				env.data.push_back(uint16_t(trigger_codes::relation_tag | association_to_trigger_code(a)));
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(value).value);
				env.data.push_back(trigger_payload(who).value);
			}
		}
	};
	struct check_variable_trigger {
		trigger_parsing_environment& env;

		float value = 0.0f;
		variables::national_variable_tag which;
		association_type a;

		check_variable_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_which(const token_and_type& t) {
			which = env.s.variables_m.get_named_national_variable(text_data::get_thread_safe_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_value(const std::pair<association_type, token_and_type>& p) {
			a = p.first;
			value = token_to<float>(p.second);
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
			env.data.push_back(uint16_t(trigger_codes::check_variable | association_to_trigger_code(a)));
			env.data.push_back(4ui16);
			add_float_to_payload(env.data, value);
			env.data.push_back(trigger_payload(which).value);
		}
	};
	struct upper_house_trigger {
		trigger_parsing_environment& env;

		float value = 0.0f;
		ideologies::ideology_tag ideology;
		association_type a;

		upper_house_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_value(const std::pair<association_type, token_and_type>& p) {
			a = p.first;
			value = token_to<float>(p.second);
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
			env.data.push_back(uint16_t(trigger_codes::upper_house | association_to_trigger_code(a)));
			env.data.push_back(4ui16);
			add_float_to_payload(env.data, value);
			env.data.push_back(trigger_payload(ideology).value);
		}
	};
	struct unemployment_by_type_trigger {
		trigger_parsing_environment& env;

		float value = 0.0f;
		population::pop_type_tag type;
		association_type a;

		unemployment_by_type_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.population_m.named_pop_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_value(const std::pair<association_type, token_and_type>& p) {
			a = p.first;
			value = token_to<float>(p.second);
		}

		void finalize() const {
			if (env.current_scope.main_slot == trigger_slot_contents::nation)
				env.data.push_back(uint16_t(trigger_codes::unemployment_by_type_nation | association_to_trigger_code(a)));
			else if (env.current_scope.main_slot == trigger_slot_contents::state)
				env.data.push_back(uint16_t(trigger_codes::unemployment_by_type_state | association_to_trigger_code(a)));
			else if(env.current_scope.main_slot == trigger_slot_contents::province)
				env.data.push_back(uint16_t(trigger_codes::unemployment_by_type_province | association_to_trigger_code(a)));
			else if(env.current_scope.main_slot == trigger_slot_contents::pop)
				env.data.push_back(uint16_t(trigger_codes::unemployment_by_type_pop | association_to_trigger_code(a)));
			else
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

			env.data.push_back(4ui16);
			add_float_to_payload(env.data, value);
			env.data.push_back(trigger_payload(type).value);
		}
	};
	struct party_loyalty_trigger {
		trigger_parsing_environment& env;

		float value = 0.0f;
		ideologies::ideology_tag ideology;
		std::optional<uint16_t> province_id;
		association_type a;

		party_loyalty_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_value(const std::pair<association_type, token_and_type>& p) {
			a = p.first;
			value = token_to<float>(p.second);
		}

		void finalize() const {
			if (province_id) {
				if(env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::party_loyalty_nation_province_id | association_to_trigger_code(a)));
				else if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::party_loyalty_from_nation_province_id | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::party_loyalty_province_province_id | association_to_trigger_code(a)));
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::party_loyalty_from_province_province_id | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

				env.data.push_back(5ui16);
				env.data.push_back(*province_id);
				add_float_to_payload(env.data, value);
				env.data.push_back(trigger_payload(ideology).value);
			} else {
				if (env.current_scope.main_slot == trigger_slot_contents::province) {
					if (env.current_scope.from_slot == trigger_slot_contents::nation)
						env.data.push_back(uint16_t(trigger_codes::party_loyalty_from_nation_scope_province | association_to_trigger_code(a)));
					else if (env.current_scope.from_slot == trigger_slot_contents::province)
						env.data.push_back(uint16_t(trigger_codes::party_loyalty_from_province_scope_province | association_to_trigger_code(a)));
					else
						TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
				} else {
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
				}
				env.data.push_back(4ui16);
				add_float_to_payload(env.data, value);
				env.data.push_back(trigger_payload(ideology).value);
			}
		}
	};
	enum class building_type_for_trigger {
		none, railroad, naval_base, fort
	};
#ifdef _DEBUG
	struct bad_building_type {};
	struct no_building_specified {};
#endif
	struct can_build_in_province_trigger {
		trigger_parsing_environment& env;

		bool limit_to_world_greatest_level = false;
		building_type_for_trigger type = building_type_for_trigger::none;

		can_build_in_province_trigger(trigger_parsing_environment& e) : env(e) {}

		void set_building(const token_and_type& t) {
			if (is_fixed_token_ci(t, "railroad"))
				type = building_type_for_trigger::railroad;
			else if (is_fixed_token_ci(t, "fort"))
				type = building_type_for_trigger::fort;
			else if (is_fixed_token_ci(t, "naval_base"))
				type = building_type_for_trigger::naval_base;
			else
				TRIGGER_ERROR(bad_building_type, env);
		}

		void finalize() const {
			if(type == building_type_for_trigger::none)
				TRIGGER_ERROR(no_building_specified, env);
			if (env.current_scope.main_slot != trigger_slot_contents::province)
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
			if (limit_to_world_greatest_level) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation) {
					if (type == building_type_for_trigger::railroad)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_railroad_yes_limit_this_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if (type == building_type_for_trigger::naval_base)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_naval_base_yes_limit_this_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if(type == building_type_for_trigger::fort)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_fort_yes_limit_this_nation | trigger_codes::association_eq | trigger_codes::no_payload));
				} else if (env.current_scope.from_slot == trigger_slot_contents::nation) {
					if (type == building_type_for_trigger::railroad)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_railroad_yes_limit_from_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if (type == building_type_for_trigger::naval_base)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_naval_base_yes_limit_from_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if (type == building_type_for_trigger::fort)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_fort_yes_limit_from_nation | trigger_codes::association_eq | trigger_codes::no_payload));
				} else {
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
				}
			} else {
				if (env.current_scope.this_slot == trigger_slot_contents::nation) {
					if (type == building_type_for_trigger::railroad)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_railroad_no_limit_this_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if (type == building_type_for_trigger::naval_base)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_naval_base_no_limit_this_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if (type == building_type_for_trigger::fort)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_fort_no_limit_this_nation | trigger_codes::association_eq | trigger_codes::no_payload));
				} else if (env.current_scope.from_slot == trigger_slot_contents::nation) {
					if (type == building_type_for_trigger::railroad)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_railroad_no_limit_from_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if (type == building_type_for_trigger::naval_base)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_naval_base_no_limit_from_nation | trigger_codes::association_eq | trigger_codes::no_payload));
					else if (type == building_type_for_trigger::fort)
						env.data.push_back(uint16_t(trigger_codes::can_build_in_province_fort_no_limit_from_nation | trigger_codes::association_eq | trigger_codes::no_payload));
				} else {
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
				}
			}
		}
	};
	struct can_build_railway_in_capital_trigger {
		trigger_parsing_environment& env;

		bool in_whole_capital_state = false;
		bool limit_to_world_greatest_level = false;

		can_build_railway_in_capital_trigger(trigger_parsing_environment& e) : env(e) {}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
			if (in_whole_capital_state) {
				if(limit_to_world_greatest_level)
					env.data.push_back(uint16_t(trigger_codes::can_build_railway_in_capital_yes_whole_state_yes_limit | trigger_codes::association_eq | trigger_codes::no_payload));
				else
					env.data.push_back(uint16_t(trigger_codes::can_build_railway_in_capital_yes_whole_state_no_limit | trigger_codes::association_eq | trigger_codes::no_payload));
			} else {
				if (limit_to_world_greatest_level)
					env.data.push_back(uint16_t(trigger_codes::can_build_railway_in_capital_no_whole_state_yes_limit | trigger_codes::association_eq | trigger_codes::no_payload));
				else
					env.data.push_back(uint16_t(trigger_codes::can_build_railway_in_capital_no_whole_state_no_limit | trigger_codes::association_eq | trigger_codes::no_payload));
			}
		}
	};
	struct can_build_fort_in_capital_trigger {
		trigger_parsing_environment& env;

		bool in_whole_capital_state = false;
		bool limit_to_world_greatest_level = false;

		can_build_fort_in_capital_trigger(trigger_parsing_environment& e) : env(e) {}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
			if (in_whole_capital_state) {
				if (limit_to_world_greatest_level)
					env.data.push_back(uint16_t(trigger_codes::can_build_fort_in_capital_yes_whole_state_yes_limit | trigger_codes::association_eq | trigger_codes::no_payload));
				else
					env.data.push_back(uint16_t(trigger_codes::can_build_fort_in_capital_yes_whole_state_no_limit | trigger_codes::association_eq | trigger_codes::no_payload));
			} else {
				if (limit_to_world_greatest_level)
					env.data.push_back(uint16_t(trigger_codes::can_build_fort_in_capital_no_whole_state_yes_limit | trigger_codes::association_eq | trigger_codes::no_payload));
				else
					env.data.push_back(uint16_t(trigger_codes::can_build_fort_in_capital_no_whole_state_no_limit | trigger_codes::association_eq | trigger_codes::no_payload));
			}
		}
	};
#ifdef _DEBUG
	struct no_worker_specified {};
#endif
	struct work_available_trigger {
		trigger_parsing_environment& env;
		std::vector<population::pop_type_tag> workers;

		work_available_trigger(trigger_parsing_environment& e) : env(e) {}

		void add_worker(const token_and_type& t) {
			workers.push_back(tag_from_text(
				env.s.population_m.named_pop_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end)));
		}

		void finalize() const {
			if(workers.size() == 0ui64)
				TRIGGER_ERROR(no_worker_specified, env);

			if (env.current_scope.main_slot == trigger_slot_contents::nation)
				env.data.push_back(uint16_t(trigger_codes::work_available_nation | trigger_codes::association_eq));
			else if (env.current_scope.main_slot == trigger_slot_contents::state)
				env.data.push_back(uint16_t(trigger_codes::work_available_state | trigger_codes::association_eq));
			else if (env.current_scope.main_slot == trigger_slot_contents::province)
				env.data.push_back(uint16_t(trigger_codes::work_available_province | trigger_codes::association_eq));
			else
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);

			env.data.push_back(uint16_t(workers.size() + 1));
			for(auto w : workers)
				env.data.push_back(trigger_payload(w).value);
		}
	};
}

MEMBER_FDEF(triggers::diplomatic_influence_trigger, set_value, "value");
MEMBER_FDEF(triggers::diplomatic_influence_trigger, set_who, "who");
MEMBER_FDEF(triggers::party_loyalty_trigger, set_value, "value");
MEMBER_FDEF(triggers::party_loyalty_trigger, set_ideology, "ideology");
MEMBER_DEF(triggers::party_loyalty_trigger, province_id, "province_id");
MEMBER_FDEF(triggers::unemployment_by_type_trigger, set_value, "value");
MEMBER_FDEF(triggers::unemployment_by_type_trigger, set_type, "type");
MEMBER_FDEF(triggers::upper_house_trigger, set_value, "value");
MEMBER_FDEF(triggers::upper_house_trigger, set_ideology, "ideology");
MEMBER_FDEF(triggers::check_variable_trigger, set_value, "value");
MEMBER_FDEF(triggers::check_variable_trigger, set_which, "which");
MEMBER_FDEF(triggers::relation_trigger, set_value, "value");
MEMBER_FDEF(triggers::relation_trigger, set_who, "who");
MEMBER_FDEF(triggers::pop_unemployment_trigger, set_value, "value");
MEMBER_FDEF(triggers::pop_unemployment_trigger, set_type_or_pop, "type");
MEMBER_DEF(triggers::can_build_in_province_trigger, limit_to_world_greatest_level, "limit_to_world_greatest_level");
MEMBER_FDEF(triggers::can_build_in_province_trigger, set_building, "building");
MEMBER_DEF(triggers::can_build_fort_in_capital_trigger, in_whole_capital_state, "in_whole_capital_state");
MEMBER_DEF(triggers::can_build_fort_in_capital_trigger, limit_to_world_greatest_level, "limit_to_world_greatest_level");
MEMBER_DEF(triggers::can_build_railway_in_capital_trigger, in_whole_capital_state, "in_whole_capital_state");
MEMBER_DEF(triggers::can_build_railway_in_capital_trigger, limit_to_world_greatest_level, "limit_to_world_greatest_level");
MEMBER_FDEF(triggers::work_available_trigger, add_worker, "worker");

namespace triggers {

#ifdef _DEBUG
	struct unknown_scope {};
#endif

	struct common_scope_base {
		trigger_parsing_environment& env;
		trigger_scope_state scope_state;
		size_t payload_size_offset = 0ui64;

		common_scope_base(trigger_parsing_environment& e) : env(e) {}

		void add_simple_trigger_f(const std::tuple<token_and_type, association_type, token_and_type>& args) {
			add_simple_trigger(
				env,
				std::get<0>(args),
				std::get<1>(args),
				std::get<2>(args));
		}
		void finalize() const {
			env.data[payload_size_offset] = uint16_t(env.data.size() - payload_size_offset);
		}
		template<typename T>
		void add_scope(const T& other) {
			other.finalize();
			env.current_scope = scope_state;
		}
		template<typename T>
		void add_complex_trigger(const T& other) {
			other.finalize();
		}
	};

}

template<typename T>
struct _set_member<CT_STRING("_add_trigger"), T> {
	template<typename V>
	static void set(T& class_passed, V&& v) {
		class_passed.add_simple_trigger_f(std::forward<V>(v));
	} 
};
template<typename T>
struct _set_member<CT_STRING("_add_complex_trigger"), T> {
	template<typename V>
	static void set(T& class_passed, V&& v) {
		class_passed.add_complex_trigger(std::forward<V>(v));
	}
};
template<typename T>
struct _set_member<CT_STRING("_add_scope"), T> {
	template<typename V>
	static void set(T& class_passed, V&& v) {
		class_passed.add_scope(std::forward<V>(v));
	}
};

#ifdef _DEBUG
struct no_code_for_scope_in_current_context {};
#endif

namespace triggers {
	struct variable_name_scope_reading_object : public common_scope_base {
		variable_name_scope_reading_object(const token_and_type& name, trigger_parsing_environment& e) : common_scope_base(e) {
			const auto left_handle = text_data::get_thread_safe_existing_text_handle(env.s.text_m, name.start, name.end);

			if (const auto region = tag_from_text(env.s.province_m.named_states_index, left_handle); is_valid_index(region)) {
				scope_state = trigger_scope_state{
					trigger_slot_contents::province,
					e.current_scope.this_slot,
					e.current_scope.from_slot,
					e.current_scope.contains_rebeltype };
				e.current_scope = scope_state;
				env.data.push_back(uint16_t(trigger_codes::x_provinces_in_variable_region | trigger_codes::is_scope));
				env.data.push_back(2ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(trigger_payload(region).value);
			} else if (const auto tag = tag_from_text(e.s.culutre_m.national_tags_index, cultures::tag_to_encoding(name.start, name.end)); is_valid_index(tag)) {
				scope_state = trigger_scope_state{
					trigger_slot_contents::nation,
					e.current_scope.this_slot,
					e.current_scope.from_slot,
					e.current_scope.contains_rebeltype };
				e.current_scope = scope_state;
				env.data.push_back(uint16_t(trigger_codes::tag_scope | trigger_codes::is_scope));
				env.data.push_back(2ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(trigger_payload(tag).value);
			} else if (is_integer(name.start, name.end)) {
				scope_state = trigger_scope_state{
					trigger_slot_contents::province,
					e.current_scope.this_slot,
					e.current_scope.from_slot,
					e.current_scope.contains_rebeltype };
				e.current_scope = scope_state;
				env.data.push_back(uint16_t(trigger_codes::integer_scope | trigger_codes::is_scope));
				env.data.push_back(2ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(token_to<uint16_t>(name));
			} else {
				TRIGGER_ERROR(unknown_scope, e);
			}
		}
	};

	template<typename scope_trigger>
	struct scope_reading_object : public common_scope_base {
		scope_reading_object(trigger_parsing_environment& e) : common_scope_base(e) {
			const auto code = scope_trigger::produce_code(e.current_scope);
			if (code) {
				e.data.push_back(uint16_t(*code | trigger_codes::is_scope));
				e.data.push_back(1ui16);
				payload_size_offset = e.data.size() - 1;
			} else {
				TRIGGER_ERROR(unknown_scope, e);
			}

			scope_state = scope_trigger::produce_new_scope(e.current_scope);
			e.current_scope = scope_state;
		}
	};

	struct factor_type_scope : public scope_reading_object<and_trigger> {
		factor_type_scope(trigger_parsing_environment& e) : scope_reading_object<and_trigger>(e) {}
		float factor = 0.0f;
	};
}

MEMBER_DEF(triggers::factor_type_scope, factor, "factor");

#define TPAIR(x) typepair< CT_STRING( #x ), x ## _trigger>

namespace triggers {
	using unsorted_trigger_map = type_list<
		TPAIR(month),
		TPAIR(port),
		TPAIR(rank),
		TPAIR(invention),
		TPAIR(strata),
		TPAIR(life_rating),
		TPAIR(has_empty_adjacent_state),
		TPAIR(state_id),
		TPAIR(cash_reserves),
		TPAIR(unemployment),
		TPAIR(is_slave),
		TPAIR(is_independant),
		TPAIR(has_national_minority),
		TPAIR(government),
		TPAIR(capital),
		TPAIR(tech_school),
		TPAIR(primary_culture),
		TPAIR(accepted_culture),
		TPAIR(culture),
		TPAIR(culture_group),
		TPAIR(religion),
		TPAIR(terrain),
		TPAIR(trade_goods),
		TPAIR(is_secondary_power),
		TPAIR(has_faction),
		TPAIR(has_unclaimed_cores),
		TPAIR(is_cultural_union),
		TPAIR(can_build_factory),
		TPAIR(war),
		TPAIR(blockade),
		TPAIR(owns),
		TPAIR(controls),
		TPAIR(is_core),
		TPAIR(num_of_revolts),
		TPAIR(revolt_percentage),
		TPAIR(num_of_cities),
		TPAIR(num_of_ports),
		TPAIR(num_of_allies),
		TPAIR(num_of_vassals),
		TPAIR(owned_by),
		TPAIR(exists),
		TPAIR(has_country_flag),
		TPAIR(continent),
		TPAIR(casus_belli),
		TPAIR(military_access),
		TPAIR(prestige),
		TPAIR(badboy),
		TPAIR(has_building),
		TPAIR(empty),
		TPAIR(is_blockaded),
		TPAIR(has_country_modifier),
		TPAIR(has_province_modifier),
		TPAIR(region),
		TPAIR(tag),
		TPAIR(neighbour),
		TPAIR(units_in_province),
		TPAIR(war_with),
		TPAIR(unit_in_battle),
		TPAIR(total_amount_of_divisions),
		TPAIR(money),
		TPAIR(lost_national),
		TPAIR(is_vassal),
		TPAIR(ruling_party_ideology),
		TPAIR(ruling_party),
		TPAIR(is_ideology_enabled),
		TPAIR(political_reform_want),
		TPAIR(social_reform_want),
		TPAIR(total_amount_of_ships),
		TPAIR(plurality),
		TPAIR(corruption),
		TPAIR(is_state_religion),
		TPAIR(is_primary_culture),
		TPAIR(is_accepted_culture),
		TPAIR(is_coastal),
		TPAIR(in_sphere),
		TPAIR(produces),
		TPAIR(average_militancy),
		TPAIR(average_consciousness),
		TPAIR(is_next_reform),
		TPAIR(rebel_power_fraction),
		TPAIR(recruited_percentage),
		TPAIR(has_culture_core),
		TPAIR(nationalism),
		TPAIR(is_overseas),
		TPAIR(controlled_by_rebels),
		TPAIR(controlled_by),
		TPAIR(is_canal_enabled),
		TPAIR(is_state_capital),
		TPAIR(truce_with),
		TPAIR(total_pops),
		TPAIR(has_pop_type),
		TPAIR(has_empty_adjacent_province),
		TPAIR(has_leader),
		TPAIR(ai),
		TPAIR(can_create_vassals),
		TPAIR(is_possible_vassal),
		TPAIR(province_id),
		TPAIR(vassal_of),
		TPAIR(alliance_with),
		TPAIR(has_recently_lost_war),
		TPAIR(is_mobilised),
		TPAIR(mobilisation_size),
		TPAIR(crime_higher_than_education),
		TPAIR(agree_with_ruling_party),
		TPAIR(is_colonial),
		TPAIR(has_factories),
		TPAIR(in_default),
		TPAIR(total_num_of_ports),
		TPAIR(always),
		TPAIR(election),
		TPAIR(has_global_flag),
		TPAIR(is_capital),
		TPAIR(nationalvalue),
		TPAIR(industrial_score),
		TPAIR(military_score),
		TPAIR(civilized),
		TPAIR(national_provinces_occupied),
		TPAIR(is_greater_power),
		TPAIR(rich_tax),
		TPAIR(middle_tax),
		TPAIR(poor_tax),
		TPAIR(social_spending),
		TPAIR(colonial_nation),
		TPAIR(pop_majority_religion),
		TPAIR(pop_majority_culture),
		TPAIR(pop_majority_issue),
		TPAIR(pop_majority_ideology),
		TPAIR(poor_strata_militancy),
		TPAIR(middle_strata_militancy),
		TPAIR(rich_strata_militancy),
		TPAIR(rich_tax_above_poor),
		TPAIR(culture_has_union_tag),
		TPAIR(this_culture_union),
		TPAIR(minorities),
		TPAIR(revanchism),
		TPAIR(has_crime),
		TPAIR(num_of_substates),
		TPAIR(num_of_vassals_no_substates),
		TPAIR(brigades_compare),
		TPAIR(constructing_cb),
		TPAIR(civilization_progress),
		TPAIR(constructing_cb_type),
		TPAIR(is_our_vassal),
		TPAIR(substate_of),
		TPAIR(is_substate),
		TPAIR(great_wars_enabled),
		TPAIR(can_nationalize),
		TPAIR(part_of_sphere),
		TPAIR(is_sphere_leader_of),
		TPAIR(number_of_states),
		TPAIR(war_score),
		TPAIR(is_releasable_vassal),
		TPAIR(has_recent_imigration),
		TPAIR(province_control_days),
		TPAIR(is_disarmed),
		TPAIR(big_producer),
		TPAIR(someone_can_form_union_tag),
		TPAIR(social_movement_strength),
		TPAIR(political_movement_strength),
		TPAIR(can_build_factory_in_capital_state),
		TPAIR(social_movement),
		TPAIR(political_movement),
		TPAIR(has_cultural_sphere),
		TPAIR(world_wars_enabled),
		TPAIR(has_pop_culture),
		TPAIR(has_pop_religion),
		TPAIR(life_needs),
		TPAIR(everyday_needs),
		TPAIR(luxury_needs),
		TPAIR(consciousness),
		TPAIR(literacy),
		TPAIR(militancy),
		TPAIR(military_spending),
		TPAIR(administration_spending),
		TPAIR(education_spending),
		TPAIR(trade_goods_in_state),
		TPAIR(has_flashpoint),
		TPAIR(flashpoint_tension),
		TPAIR(crisis_exist),
		TPAIR(is_liberation_crisis),
		TPAIR(is_claim_crisis),
		TPAIR(crisis_temperature),
		TPAIR(involved_in_crisis),
		TPAIR(poor_strata_luxury_needs),
		TPAIR(middle_strata_luxury_needs),
		TPAIR(rich_strata_luxury_needs),
		TPAIR(poor_strata_life_needs),
		TPAIR(middle_strata_life_needs),
		TPAIR(rich_strata_life_needs),
		TPAIR(poor_strata_everyday_needs),
		TPAIR(middle_strata_everyday_needs),
		TPAIR(rich_strata_everyday_needs),
		typepair<CT_STRING("country_units_in_province"), units_in_province_trigger>,
		typepair<CT_STRING("pop_type"), has_pop_type_trigger>
	>;

	using trigger_map = typename sorted<unsorted_trigger_map>::type;

	//const trigger_scope_state& scope, association_type a, const token_and_type&
	void add_simple_trigger(
		trigger_parsing_environment& env,
		const token_and_type& trigger_name,
		association_type a,
		const token_and_type& trigger_value) {

		if (!map_call_functions<trigger_map>::template bt_scan_ci<bool>(trigger_name.start, trigger_name.end,
			[a, &trigger_value, &env](auto type_arg) {
			const auto code = decltype(type_arg)::type::produce_code(env.current_scope, a, trigger_value);
			if (code) {
				const auto data_size = code_data_sizes[*code & trigger_codes::code_mask];
				if (data_size == 0) {
					env.data.push_back(uint16_t(*code | trigger_codes::no_payload));
				} else {
					env.data.push_back(*code);
					env.data.push_back(static_cast<uint16_t>(data_size + 1));

					const auto payload = decltype(type_arg)::type::read_value(trigger_value, env.s);
					//using trigger_value = std::variant<std::monostate, int32_t, float, issues::option_identifier, trigger_payload>;
					if (std::holds_alternative<int32_t>(payload)) {
						add_int32_t_to_payload(env.data, std::get<int32_t>(payload));
#ifdef _DEBUG
						if (data_size != 2)
							throw mismatched_payload_size();
#endif
					} else if (std::holds_alternative<float>(payload)) {
						add_float_to_payload(env.data, std::get<float>(payload));
#ifdef _DEBUG
						if (data_size != 2)
							throw mismatched_payload_size();
#endif
					} else if (std::holds_alternative<issues::option_identifier>(payload)) {
						add_option_identifier_to_payload(env.data, std::get<issues::option_identifier>(payload));
#ifdef _DEBUG
						if (data_size != 2)
							throw mismatched_payload_size();
#endif
					} else if (std::holds_alternative<trigger_payload>(payload)) {
						env.data.push_back(std::get<trigger_payload>(payload).value);
#ifdef _DEBUG
						if (data_size != 1)
							throw mismatched_payload_size();
#endif
					} else {
#ifdef _DEBUG
						throw no_payload_value();
#endif
					}
				}
			} else {
				TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
			}
			return true;
		})) {
			//not found in map
			const auto left_handle = text_data::get_thread_safe_existing_text_handle(env.s.text_m, trigger_name.start, trigger_name.end);
			//...
			
			if (const auto tech = tag_from_text(env.s.technology_m.named_technology_index, left_handle); is_valid_index(tech)) {
				if (env.current_scope.main_slot != trigger_slot_contents::nation) {
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, e);
				} else {
					env.data.push_back(uint16_t(trigger_codes::variable_tech_name | association_to_bool_code(a, token_to<int32_t>(trigger_value) >= 1)));
					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(tech).value);
				}
			} else if (const auto ideology = tag_from_text(env.s.ideologies_m.named_ideology_index, left_handle); is_valid_index(ideology)) {
				if(env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::variable_ideology_name_nation | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::pop)
					env.data.push_back(uint16_t(trigger_codes::variable_ideology_name_pop | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::variable_ideology_name_province | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::state)
					env.data.push_back(uint16_t(trigger_codes::variable_ideology_name_state | association_to_trigger_code(a)));
				else 
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, e);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(ideology).value);
				add_float_to_payload(env.data, token_to<float>(trigger_value));
			} else if (const auto ptype = tag_from_text(env.s.population_m.named_pop_type_index, left_handle); is_valid_index(ptype)) {
				if (env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::variable_pop_type_name_nation | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::pop)
					env.data.push_back(uint16_t(trigger_codes::variable_pop_type_name_pop | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::variable_pop_type_name_province | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::state)
					env.data.push_back(uint16_t(trigger_codes::variable_pop_type_name_state | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(ptype).value);
				add_float_to_payload(env.data, token_to<float>(trigger_value));
			} else if (const auto good = tag_from_text(env.s.economy_m.named_goods_index, left_handle); is_valid_index(good)) {
				if (env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::variable_good_name | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(good).value);
				add_float_to_payload(env.data, token_to<float>(trigger_value));
			} else if (const auto issue_opt = tag_from_text(env.s.issues_m.named_option_index, left_handle); !std::holds_alternative<std::monostate>(issue_opt.id)) {
				if (env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_name_nation | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::pop)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_name_pop | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_name_province | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::state)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_name_state | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, env);
				env.data.push_back(5ui16);
				add_option_identifier_to_payload(env.data, issue_opt);
				add_float_to_payload(env.data, token_to<float>(trigger_value));
			} else if (const auto issue = tag_from_text(env.s.issues_m.named_issue_index, left_handle); !std::holds_alternative<std::monostate>(issue.id)) {
				if (env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_group_name_nation | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::pop)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_group_name_pop | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_group_name_province | association_to_trigger_code(a)));
				else if (env.current_scope.main_slot == trigger_slot_contents::state)
					env.data.push_back(uint16_t(trigger_codes::variable_issue_group_name_state | association_to_trigger_code(a)));
				else
					TRIGGER_ERROR(no_code_value_found_for_scope_and_argument, e);
				const auto right_handle = text_data::get_thread_safe_existing_text_handle(env.s.text_m, trigger_value.start, trigger_value.end);
				const auto rh_issue_opt = tag_from_text(env.s.issues_m.named_option_index, right_handle);

				env.data.push_back(3ui16);
				add_option_identifier_to_payload(env.data, rh_issue_opt);
			} else {
				TRIGGER_ERROR(unknown_trigger, env);
			}
		}
	}

	inline variable_name_scope_reading_object& get_vscope(const token_and_type&, association_type, variable_name_scope_reading_object& t) {
		return t;
	};


	BEGIN_DOMAIN(trigger_reading)
		BEGIN_TYPE(common_scope_base)
		    MEMBER_VARIABLE_ASSOCIATION("_add_trigger", accept_all, yield_full)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "diplomatic_influence", diplomatic_influence_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "pop_unemployment", pop_unemployment_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "relation", relation_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "check_variable", check_variable_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "upper_house", upper_house_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "unemployment_by_type", unemployment_by_type_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "party_loyalty", party_loyalty_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "can_build_in_province", can_build_in_province_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "can_build_railway_in_capital", can_build_railway_in_capital_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "can_build_fort_in_capital", can_build_fort_in_capital_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_trigger", "work_available", work_available_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "and", scope_reading_object<and_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "or", scope_reading_object<or_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "not", scope_reading_object<not_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_neighbor_province", scope_reading_object<any_neighbor_province_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_neighbor_country", scope_reading_object<any_neighbor_country_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "war_countries", scope_reading_object<war_countries_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_greater_power", scope_reading_object<any_greater_power_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_owned_province", scope_reading_object<any_owned_province_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_core", scope_reading_object<any_core_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "all_core", scope_reading_object<all_core_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_state", scope_reading_object<any_state_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_substate", scope_reading_object<any_substate_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_sphere_member", scope_reading_object<any_sphere_member_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_pop", scope_reading_object<any_pop_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "owner", scope_reading_object<owner_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "controller", scope_reading_object<controller_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "location", scope_reading_object<location_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "country", scope_reading_object<country_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "capital_scope", scope_reading_object<capital_scope_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "this", scope_reading_object<this_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "from", scope_reading_object<from_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "sea_zone", scope_reading_object<sea_zone_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "cultural_union", scope_reading_object<cultural_union_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "overlord", scope_reading_object<overlord_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "sphere_owner", scope_reading_object<sphere_owner_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "independence", scope_reading_object<independence_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "flashpoint_tag_scope", scope_reading_object<flashpoint_tag_scope_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "crisis_state_scope", scope_reading_object<crisis_state_scope_trigger>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "state_scope", scope_reading_object<state_scope_trigger>)
			MEMBER_VARIABLE_TYPE_ASSOCIATION("_add_scope", accept_all, variable_name_scope_reading_object, get_vscope)
		END_TYPE
		BEGIN_TYPE(diplomatic_influence_trigger)
		    MEMBER_ASSOCIATION("value", "value", yield_rh)
		    MEMBER_ASSOCIATION("who", "who", token_from_rh)
		END_TYPE
		BEGIN_TYPE(party_loyalty_trigger)
			MEMBER_ASSOCIATION("value", "value", yield_rh)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("province_id", "province_id", value_from_rh<uint16_t>)
		END_TYPE
		BEGIN_TYPE(unemployment_by_type_trigger)
			MEMBER_ASSOCIATION("value", "value", yield_rh)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
		END_TYPE
		BEGIN_TYPE(upper_house_trigger)
			MEMBER_ASSOCIATION("value", "value", yield_rh)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
		END_TYPE
		BEGIN_TYPE(check_variable_trigger)
			MEMBER_ASSOCIATION("value", "value", yield_rh)
			MEMBER_ASSOCIATION("which", "which", token_from_rh)
		END_TYPE
		BEGIN_TYPE(relation_trigger)
			MEMBER_ASSOCIATION("value", "value", yield_rh)
			MEMBER_ASSOCIATION("who", "who", token_from_rh)
		END_TYPE
		BEGIN_TYPE(pop_unemployment_trigger)
			MEMBER_ASSOCIATION("value", "value", yield_rh)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("type", "pop", token_from_rh)
		END_TYPE
		BEGIN_TYPE(can_build_in_province_trigger)
			MEMBER_ASSOCIATION("limit_to_world_greatest_level", "limit_to_world_greatest_level", value_from_rh<bool>)
			MEMBER_ASSOCIATION("building", "building", token_from_rh)
		END_TYPE
		BEGIN_TYPE(can_build_fort_in_capital_trigger)
			MEMBER_ASSOCIATION("limit_to_world_greatest_level", "limit_to_world_greatest_level", value_from_rh<bool>)
			MEMBER_ASSOCIATION("in_whole_capital_state", "in_whole_capital_state", value_from_rh<bool>)
		END_TYPE
		BEGIN_TYPE(can_build_railway_in_capital_trigger)
			MEMBER_ASSOCIATION("limit_to_world_greatest_level", "limit_to_world_greatest_level", value_from_rh<bool>)
			MEMBER_ASSOCIATION("in_whole_capital_state", "in_whole_capital_state", value_from_rh<bool>)
		END_TYPE
		BEGIN_TYPE(work_available_trigger)
		    MEMBER_ASSOCIATION("worker", "worker", token_from_rh)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<and_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<or_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<not_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_neighbor_province_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_neighbor_country_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<war_countries_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_greater_power_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_owned_province_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_core_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<all_core_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_state_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_substate_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_sphere_member_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<any_pop_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<owner_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<controller_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<location_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<country_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<capital_scope_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<this_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<from_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<sea_zone_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<cultural_union_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<overlord_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<sphere_owner_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<independence_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<flashpoint_tag_scope_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<crisis_state_scope_trigger>)
		    INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(scope_reading_object<state_scope_trigger>)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(variable_name_scope_reading_object)
			INHERIT_FROM(common_scope_base)
		END_TYPE
		BEGIN_TYPE(factor_type_scope)
			INHERIT_FROM(common_scope_base)
			MEMBER_ASSOCIATION("factor", "factor", value_from_rh<float>)
		END_TYPE
	END_DOMAIN;

	inline void invert_trigger_internal(uint16_t* source) {
		if ((source[0] & trigger_codes::is_scope) != 0) {
			const auto neg_disjunctive_bit = trigger_codes::is_disjunctive_scope & ~source[0];
			const auto neg_existence_bit = scope_has_any_all(source[0] & trigger_codes::code_mask) ? (trigger_codes::is_existance_scope & ~source[0]) : 0;
			const auto masked_source = source[0] & ~(trigger_codes::is_disjunctive_scope | trigger_codes::is_existance_scope);

			source[0] = uint16_t(masked_source | neg_disjunctive_bit | neg_existence_bit);
		} else {
			const auto inverse_association = invert_association(uint16_t(source[0] & trigger_codes::association_mask));
			source[0] = uint16_t((source[0] & ~trigger_codes::association_mask) | inverse_association);
		}
	}

	void invert_trigger(uint16_t* source) {
		recurse_over_triggers(source, invert_trigger_internal);
	}

	bool scope_is_empty(const uint16_t* source) {
		return get_trigger_payload_size(source) <= 1 + trigger_scope_data_payload(source[0]);
	}
	//precondition: scope known to not be empty
	bool scope_has_single_member(const uint16_t* source) {
		const auto data_offset = 2 + trigger_scope_data_payload(source[0]);
		return get_trigger_payload_size(source) == data_offset + get_trigger_payload_size(source + data_offset);
	}

	//yields new source size
	int32_t simplify_trigger(uint16_t* source) {
		if ((source[0] & trigger_codes::is_scope) != 0) {
			if (scope_is_empty(source)) {
				return 0; // simplify an empty scope to nothing
			}

			//simplify each member
			auto source_size = 1 + get_trigger_payload_size(source);

			const auto first_member = source + 2 + trigger_scope_data_payload(source[0]);
			auto sub_units_start = first_member;

			while (sub_units_start < source + source_size) {
				const auto old_size = 1 + get_trigger_payload_size(sub_units_start);
				const auto new_size = simplify_trigger(sub_units_start);
				
				if (new_size != old_size) { // has been simplified, assumes that new size always <= old size
					std::copy(sub_units_start + old_size, source + source_size, sub_units_start + new_size);
					source_size -= (old_size - new_size);
				}
				sub_units_start += new_size;
			}

			source[1] = uint16_t(source_size - 1);

			if ((source[0] & trigger_codes::code_mask) == trigger_codes::placeholder_not_scope) { // remove not scopes
				invert_trigger(source);
				source[0] = uint16_t((source[0] & ~trigger_codes::code_mask) | trigger_codes::generic_scope);
			}

			if (scope_has_single_member(source)) {
				

				if ((source[0] & trigger_codes::code_mask) == trigger_codes::generic_scope) { // remove single-member generic scopes
					std::copy(source + 2, source + source_size, source);
					source_size -= 2;
				} else if((first_member[0] & (trigger_codes::code_mask | trigger_codes::is_scope)) == (trigger_codes::generic_scope | trigger_codes::is_scope)) {
					// scope contains single generic scope
					
					source[1] = uint16_t(first_member[1] + trigger_scope_data_payload(source[0]));
					source[0] = uint16_t((source[0] & ~trigger_codes::is_disjunctive_scope) | (first_member[0] & trigger_codes::is_disjunctive_scope));

					std::copy(first_member + 2, source + source_size, first_member);
					
					source_size -= 2;
				}
			}

			return source_size;
		} else {
			return 1 + get_trigger_payload_size(source); // non scopes cannot be simplified
		}
	}

	std::vector<uint16_t> parse_trigger(scenario::scenario_manager& s, trigger_scope_state outer_scope, const token_group* start, const token_group* end) {
		trigger_parsing_environment parse_env(s, outer_scope);

		auto trigger = parse_object<scope_reading_object<and_trigger>, trigger_reading>(start, end, parse_env);
		trigger.finalize();
		parse_env.data.push_back(0ui16);

		const auto new_size = simplify_trigger(parse_env.data.data());
		parse_env.data.resize(static_cast<size_t>(new_size));
		return std::move(parse_env.data);
	}

	trigger_and_factor parse_trigger_and_factor(scenario::scenario_manager& s, trigger_scope_state outer_scope, const token_group* start, const token_group* end) {
		trigger_and_factor result;
		trigger_parsing_environment parse_env(s, outer_scope);

		auto trigger = parse_object<factor_type_scope, trigger_reading>(start, end, parse_env);
		trigger.finalize();
		parse_env.data.push_back(0ui16);

		const auto new_size = simplify_trigger(parse_env.data.data());
		parse_env.data.resize(static_cast<size_t>(new_size));
		
		result.data = std::move(parse_env.data);
		result.factor = trigger.factor;

		return result;
	}

	trigger_tag commit_trigger(trigger_manager& trigger_manager, const std::vector<uint16_t>& new_trigger) {
		if (new_trigger.size() == 0ui64)
			return trigger_tag();

		const auto search_result = std::search(trigger_manager.trigger_data.begin(), trigger_manager.trigger_data.end(), new_trigger.begin(), new_trigger.end());
		if (search_result != trigger_manager.trigger_data.end()) {
			return trigger_tag(static_cast<value_base_of<trigger_tag>>(search_result - trigger_manager.trigger_data.begin()));
		} else {
			trigger_manager.trigger_data.pop_back();
			const auto new_start_pos = trigger_manager.trigger_data.size();
			trigger_manager.trigger_data.insert(trigger_manager.trigger_data.end(), new_trigger.begin(), new_trigger.end());
			trigger_manager.trigger_data.push_back(0ui16);
			return trigger_tag(static_cast<value_base_of<trigger_tag>>(new_start_pos));
		}
	}
}
