#include "triggers.h"
#include "codes.h"
#include "scenario\\scenario.h"
#include <optional>
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"

namespace triggers {
	namespace pop_strata {
		constexpr int32_t poor = 0;
		constexpr int32_t middle = 1;
		constexpr int32_t rich = 2;
	}

	struct year_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::year | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct is_canal_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::is_canal_enabled | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct month_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::month | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct great_wars_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::great_wars_enabled | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct world_wars_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::world_wars_enabled | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct crisis_exist_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::crisis_exist | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_liberation_crisis_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::is_liberation_crisis | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_claim_crisis_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::is_claim_crisis | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};

	struct port_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::port | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct involved_in_crisis_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::involved_in_crisis_pop | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::involved_in_crisis_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_cultural_sphere_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_cultural_sphere | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct social_movement_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::social_movement | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct political_movement_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::political_movement | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct rich_tax_above_poor_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_tax_above_poor | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_substate_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_substate | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_flashpoint_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_flashpoint | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_disarmed_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_disarmed | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct can_nationalize_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::can_nationalize | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct part_of_sphere_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::part_of_sphere | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct constructing_cb_discovered_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::constructing_cb_discovered | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct colonial_nation_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::colonial_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_capital_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_capital | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct election_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::election | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct always_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::always | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_releasable_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_releasable_vassal_from | association_to_trigger_code(a)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_releasable_vassal_other | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct someone_can_form_union_tag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::someone_can_form_union_tag_from | association_to_trigger_code(a)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::someone_can_form_union_tag_other | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_state_capital_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_state_capital | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_factories_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_factories | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_empty_adjacent_province_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_empty_adjacent_province | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct ai_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::ai | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct minorities_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::minorities_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::minorities_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::minorities_province | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct culture_has_union_tag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::culture_has_union_tag_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::culture_has_union_tag_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_colonial_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_colonial_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::is_colonial_state | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_greater_power_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_greater_power_pop | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_greater_power_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct can_create_vassals_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::can_create_vassals | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_recently_lost_war_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_recently_lost_war | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_mobilised_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_mobilised | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct crime_higher_than_education_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct civilized_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::civilized_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::civilized_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::civilized_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};

	struct rank_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rank | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct crisis_temperature_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::crisis_temperature | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct has_recent_imigration_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_recent_imigration | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct province_control_days_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::province_control_days | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_substates_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_substates | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_vassals_no_substates_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_vassals_no_substates | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct number_of_states_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::number_of_states | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct war_score_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::war_score | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct flashpoint_tension_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::flashpoint_tension | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct life_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::war_score | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct everyday_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::everyday_needs | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct luxury_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::luxury_needs | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct social_movement_strength_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::social_movement_strength | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct political_movement_strength_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::political_movement_strength | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct total_num_of_ports_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_num_of_ports | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct agree_with_ruling_party_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::agree_with_ruling_party | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct constructing_cb_progress_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::constructing_cb_progress | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct civilization_progress_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::civilization_progress | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct rich_strata_life_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_strata_life_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::rich_strata_life_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::rich_strata_life_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::rich_strata_life_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct rich_strata_everyday_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_strata_everyday_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::rich_strata_everyday_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::rich_strata_everyday_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::rich_strata_everyday_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct rich_strata_luxury_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_strata_luxury_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::rich_strata_luxury_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::rich_strata_luxury_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::rich_strata_luxury_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	
	struct middle_strata_life_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::middle_strata_life_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::middle_strata_life_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::middle_strata_life_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::middle_strata_life_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct middle_strata_everyday_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::middle_strata_everyday_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::middle_strata_everyday_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::middle_strata_everyday_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::middle_strata_everyday_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct middle_strata_luxury_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::middle_strata_luxury_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::middle_strata_luxury_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::middle_strata_luxury_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::middle_strata_luxury_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};

	struct poor_strata_life_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::poor_strata_life_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::poor_strata_life_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::poor_strata_life_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::poor_strata_life_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct poor_strata_everyday_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::poor_strata_everyday_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::poor_strata_everyday_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::poor_strata_everyday_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::poor_strata_everyday_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct poor_strata_luxury_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::poor_strata_luxury_needs_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::poor_strata_luxury_needs_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::poor_strata_luxury_needs_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::poor_strata_luxury_needs_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};

	struct revanchism_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::revanchism_pop | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::revanchism_nation | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct poor_strata_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct middle_strata_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct rich_strata_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct consciousness_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::consciousness_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::consciousness_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::consciousness_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::consciousness_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct literacy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::literacy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::literacy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::literacy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::literacy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct military_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::military_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::military_spending_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::military_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::military_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct administration_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::administration_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::administration_spending_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::administration_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::administration_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct education_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::education_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::education_spending_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::education_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::education_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct national_provinces_occupied_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::national_provinces_occupied | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct social_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::social_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::social_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::social_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct brigades_compare_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (scope.this_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::brigades_compare_this | association_to_trigger_code(a)), 1ui16 };
				else if (scope.from_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::brigades_compare_from | association_to_trigger_code(a)), 1ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct rich_tax_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_tax | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct middle_tax_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::middle_tax | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct poor_tax_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::poor_tax | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct mobilisation_size_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::mobilisation_size | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct province_id_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::province_id | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct invention_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::invention | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.technology_m.named_invention_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct big_producer_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::big_producer | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct strata_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::strata | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			if (is_fixed_token_ci(t.start, t.end, "poor")) {
				return trigger_bytecode(pop_strata::poor);
			} else if (is_fixed_token_ci(t.start, t.end, "rich")) {
				return trigger_bytecode(pop_strata::rich);
			} else {
				return trigger_bytecode(pop_strata::middle);
			}
		}
	};
	struct life_rating_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::life_rating_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::life_rating_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct has_empty_adjacent_state_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_empty_adjacent_state_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_empty_adjacent_state_state | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct state_id_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::state_id_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::state_id_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct cash_reserves_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::cash_reserves | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct unemployment_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::unemployment_pop | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::unemployment_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::unemployment_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::unemployment_nation | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct is_slave_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_slave_pop | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::is_slave_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_slave_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_slave_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_independant_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_independant | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_national_minority_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_national_minority_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_national_minority_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_national_minority_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct government_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::government_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::government_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.governments_m.named_government_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct constructing_cb_type_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::constructing_cb_type | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.military_m.named_cb_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct can_build_factory_in_capital_state_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::can_build_factory_in_capital_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_factory_types_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct capital_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::capital | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct tech_school_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::tech_school | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.technology_m.named_tech_school_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct primary_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::primary_culture | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_crime_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_crime | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct accepted_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::accepted_culture | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::pop_majority_religion_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::pop_majority_religion_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::pop_majority_religion_province | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::pop_majority_culture_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::pop_majority_culture_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::pop_majority_culture_province | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_issue_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::pop_majority_issue_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::pop_majority_issue_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::pop_majority_issue_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::pop_majority_issue_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.issues_m.named_option_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_ideology_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::pop_majority_ideology_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::pop_majority_ideology_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::pop_majority_ideology_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::pop_majority_ideology_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct trade_goods_in_state_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::trade_goods_in_state_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::trade_goods_in_state_province | association_to_trigger_code(a)), 1ui16 };

			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::culture_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::culture_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::culture_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::culture_this_province | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else if (is_fixed_token_ci(t, "from")) {
				if (scope.main_slot == trigger_slot_contents::pop && scope.from_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::culture_from_nation | association_to_trigger_code(a)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else if (is_fixed_token_ci(t, "reb")) {
				if (scope.contains_rebeltype == false)
					return std::optional<trigger_code>();
				else if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::culture_pop_reb | association_to_trigger_code(a)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ uint16_t(codes::culture_state_reb | association_to_trigger_code(a)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ uint16_t(codes::culture_province_reb | association_to_trigger_code(a)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::culture_nation_reb | association_to_trigger_code(a)), 0ui16 };
			}
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::culture_pop | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::culture_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::culture_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::culture_nation | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.this_slot == trigger_slot_contents::pop) {
					if (scope.main_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::has_pop_culture_nation_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::has_pop_culture_pop_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::has_pop_culture_state_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::has_pop_culture_province_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::has_pop_culture_pop | association_to_trigger_code(a)), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ uint16_t(codes::has_pop_culture_state | association_to_trigger_code(a)), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ uint16_t(codes::has_pop_culture_province | association_to_trigger_code(a)), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::has_pop_culture_nation | association_to_trigger_code(a)), 1ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.this_slot == trigger_slot_contents::pop) {
					if (scope.main_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::has_pop_religion_nation_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::has_pop_religion_pop_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::has_pop_religion_state_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::has_pop_religion_province_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::has_pop_religion_pop | association_to_trigger_code(a)), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ uint16_t(codes::has_pop_religion_state | association_to_trigger_code(a)), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ uint16_t(codes::has_pop_religion_province | association_to_trigger_code(a)), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::has_pop_religion_nation | association_to_trigger_code(a)), 1ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct culture_group_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::culture_group_pop_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::culture_group_pop_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::culture_group_pop_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::culture_group_pop_this_province | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::nation) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::culture_group_nation_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::culture_group_nation_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::culture_group_nation_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::culture_group_nation_this_province | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation) {
					if (scope.main_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::culture_group_pop_from_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::culture_group_nation_from_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else if (is_fixed_token_ci(t, "reb")) {
				if (scope.contains_rebeltype == false)
					return std::optional<trigger_code>();
				else if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::culture_group_reb_pop | association_to_trigger_code(a)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::culture_group_reb_nation | association_to_trigger_code(a)), 0ui16 };
			}
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::culture_group_pop | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::culture_group_nation | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::religion_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::religion_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::religion_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::religion_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::religion_from_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "reb")) {
					return trigger_code{ uint16_t(codes::religion_reb | association_to_trigger_code(a)), 0ui16 };
				} else {
					return trigger_code{ uint16_t(codes::religion | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct terrain_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop) {
				return trigger_code{ uint16_t(codes::terrain_pop | association_to_trigger_code(a)), 1ui16 };
			} else if (scope.main_slot == trigger_slot_contents::province) {
				return trigger_code{ uint16_t(codes::terrain_province | association_to_trigger_code(a)), 1ui16 };
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct trade_goods_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::trade_goods | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct is_secondary_power_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_secondary_power_pop | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_secondary_power_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_faction_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::has_faction_pop | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_faction_nation | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.population_m.named_rebel_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_unclaimed_cores_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_unclaimed_cores | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_cultural_union_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes") || is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_cultural_union_bool | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::is_cultural_union_this_self_pop | association_to_trigger_code(a)), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::is_cultural_union_this_pop | association_to_trigger_code(a)), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::state)
					return trigger_code{ uint16_t(codes::is_cultural_union_this_state | association_to_trigger_code(a)), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::province)
					return trigger_code{ uint16_t(codes::is_cultural_union_this_province | association_to_trigger_code(a)), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_cultural_union_this_nation | association_to_trigger_code(a)), 0ui16 };
				else if (scope.contains_rebeltype)
					return trigger_code{ uint16_t(codes::is_cultural_union_this_rebel | association_to_trigger_code(a)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_cultural_union_tag_nation | association_to_trigger_code(a)), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::is_cultural_union_tag_this_pop | association_to_trigger_code(a)), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::state)
					return trigger_code{ uint16_t(codes::is_cultural_union_tag_this_state | association_to_trigger_code(a)), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::province)
					return trigger_code{ uint16_t(codes::is_cultural_union_tag_this_province | association_to_trigger_code(a)), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_cultural_union_tag_this_nation | association_to_trigger_code(a)), 1ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct can_build_factory_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::can_build_factory | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct war_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::war_pop | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::war_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct war_exhaustion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::war_exhaustion | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct blockade_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::blockade | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct owns_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::owns | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct controls_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::controls | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct is_core_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_integer(t.start, t.end)) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_core_integer | association_to_trigger_code(a)), 1ui16 };
				else
					return std::optional<trigger_code>();
			} else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_core_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_core_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_core_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_core_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_core_from_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return trigger_code{ uint16_t(codes::is_core_reb | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::is_core_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			if (is_integer(t.start, t.end)) {
				return trigger_bytecode{ token_to<int32_t>(t) };
			} else {
				return trigger_bytecode(
					tag_from_text(
						s.culutre_m.national_tags_index,
						cultures::tag_to_encoding(t.start, t.end)));
			}
		}
	};
	struct num_of_revolts_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_revolts | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct revolt_percentage_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::revolt_percentage | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct num_of_cities_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::num_of_cities_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::num_of_cities_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::num_of_cities_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::num_of_cities_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::num_of_cities_from_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::num_of_cities_int | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_ports_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_ports | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_allies_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_allies | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_vassals_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_vassals | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct owned_by_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::owned_by_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::owned_by_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::owned_by_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::owned_by_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::owned_by_from_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::owned_by_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct exists_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes") | is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::exists_bool | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				return trigger_code{ uint16_t(codes::exists_tag | association_to_trigger_code(a)), 1ui16 };
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct has_country_flag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_country_flag | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_bytecode{
				s.variables_m.get_named_national_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct has_global_flag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::has_global_flag | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_bytecode{
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct continent_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_nation_this | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_nation_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::continent_nation | association_to_trigger_code(a)), 1ui16 };
				}
			} else if (scope.main_slot == trigger_slot_contents::state) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_state_this | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_state_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::continent_state | association_to_trigger_code(a)), 1ui16 };
				}
			} else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_province_this | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_province_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::continent_province | association_to_trigger_code(a)), 1ui16 };
				}
			} else if (scope.main_slot == trigger_slot_contents::pop) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_pop_this | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::continent_pop_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::continent_pop | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct casus_belli_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::casus_belli_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::casus_belli_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::casus_belli_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::casus_belli_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::casus_belli_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::casus_belli_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct military_access_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::military_access_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::military_access_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::military_access_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::military_access_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::military_access_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::military_access_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct prestige_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::prestige_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::prestige_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::prestige_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::prestige_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::prestige_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::prestige_value | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct badboy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::badboy | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct has_building_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_building_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "fort"))
					return trigger_code{ uint16_t(codes::has_building_fort | association_to_trigger_code(a)), 0ui16 };
				else if (is_fixed_token_ci(t, "railroad"))
					return trigger_code{ uint16_t(codes::has_building_railroad | association_to_trigger_code(a)), 0ui16 };
				else if (is_fixed_token_ci(t, "naval_base"))
					return trigger_code{ uint16_t(codes::has_building_naval_base | association_to_trigger_code(a)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_factory_types_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct empty_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::empty | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_blockaded_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_blockaded | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_country_modifier_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_country_modifier | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_province_modifier_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_province_modifier | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct nationalvalue_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::nationalvalue | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct region_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::region | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.province_m.named_states_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct tag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::tag_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::tag_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::tag_from_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.from_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::tag_from_province | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::tag_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct neighbour_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::neighbour_this | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::neighbour_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::neighbour_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct units_in_province_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::units_in_province_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::units_in_province_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::units_in_province_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::units_in_province_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::units_in_province_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::units_in_province_value | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<int32_t>(t));
		}
	};
	struct war_with_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::war_with_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::war_with_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::war_with_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::war_with_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::war_with_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::war_with_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct unit_in_battle_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::unit_in_battle | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct total_amount_of_divisions_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_amount_of_divisions | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct money_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::money | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct lost_national_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::lost_national | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct is_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_vassal | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct ruling_party_ideology_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::ruling_party_ideology_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::ruling_party_ideology_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct ruling_party_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::ruling_party | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct has_leader_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_leader | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct is_ideology_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::is_ideology_enabled | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct political_reform_want_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::political_reform_want_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::political_reform_want_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct social_reform_want_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::social_reform_want_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::social_reform_want_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct total_amount_of_ships_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_amount_of_ships | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct plurality_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::plurality | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct corruption_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::corruption | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct is_state_religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::is_state_religion_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_state_religion_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_state_religion_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};

	struct is_primary_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::nation) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::state) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::province) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ uint16_t(codes::is_primary_culture_state | association_to_bool_code(a, t)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ uint16_t(codes::is_primary_culture_province | association_to_bool_code(a, t)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::is_primary_culture_pop | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_accepted_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::is_accepted_culture_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_accepted_culture_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_accepted_culture_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();

		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_coastal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_coastal | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct in_sphere_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::in_sphere_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::in_sphere_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::in_sphere_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::in_sphere_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::in_sphere_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::in_sphere_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct produces_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::produces_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::produces_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::produces_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::produces_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_type_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_pop_type_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_pop_type_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_pop_type_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::has_pop_type_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.population_m.named_pop_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct total_pops_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_pops_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::total_pops_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::total_pops_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::total_pops_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<int32_t>(t));
		}
	};
	struct average_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::average_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::average_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::average_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct average_consciousness_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::average_consciousness_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::average_consciousness_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::average_consciousness_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct is_next_reform_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_next_reform_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_next_reform_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.issues_m.named_option_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct rebel_power_fraction_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rebel_power_fraction | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct recruited_percentage_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::recruited_percentage_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::recruited_percentage_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct has_culture_core_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::has_culture_core | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct nationalism_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::nationalism | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<int32_t>(t));
		}
	};
	struct is_overseas_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_overseas | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct controlled_by_rebels_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::controlled_by_rebels | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct controlled_by_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::controlled_by_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::controlled_by_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::controlled_by_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::controlled_by_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::controlled_by_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "owner")) {
					return trigger_code{ uint16_t(codes::controlled_by_owner | association_to_trigger_code(a)), 0ui16 };
				} else if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return trigger_code{ uint16_t(codes::controlled_by_reb | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::controlled_by_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct truce_with_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::truce_with_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::truce_with_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::truce_with_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::truce_with_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::truce_with_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::truce_with_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct is_sphere_leader_of_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_sphere_leader_of_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_sphere_leader_of_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_sphere_leader_of_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_sphere_leader_of_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_sphere_leader_of_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::is_sphere_leader_of_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct constructing_cb_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::constructing_cb_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::constructing_cb_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::constructing_cb_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::constructing_cb_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::constructing_cb_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::constructing_cb_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct vassal_of_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::vassal_of_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::vassal_of_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::vassal_of_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::vassal_of_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::vassal_of_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::vassal_of_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct substate_of_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::substate_of_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::substate_of_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::substate_of_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::substate_of_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::substate_of_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::substate_of_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct is_our_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_our_vassal_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_our_vassal_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_our_vassal_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_our_vassal_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_our_vassal_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::is_our_vassal_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct this_culture_union_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::this_culture_union_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::this_culture_union_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::this_culture_union_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::this_culture_union_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "this_union")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::this_culture_union_this_union_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::this_culture_union_this_union_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::this_culture_union_this_union_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::this_culture_union_this_union_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::this_culture_union_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::this_culture_union_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct alliance_with_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::alliance_with_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::alliance_with_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::alliance_with_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::alliance_with_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::alliance_with_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::alliance_with_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct in_default_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::in_default_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::in_default_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::in_default_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::in_default_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::in_default_from | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::in_default_tag | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct industrial_score_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::industrial_score_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::industrial_score_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::industrial_score_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::industrial_score_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::industrial_score_from_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::industrial_score_value | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct military_score_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::military_score_this_pop | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::military_score_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::military_score_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::military_score_this_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::military_score_from_nation | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ uint16_t(codes::military_score_value | association_to_trigger_code(a)), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct is_possible_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_possible_vassal | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};


	//for scope triggers
	struct or_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&) {
			return std::optional<trigger_code>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};

#define TPAIR(x) typepair< CT_STRING( #x ), x ## _trigger>

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
}
