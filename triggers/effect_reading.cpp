#include "effect_reading.h"
#include "codes.h"
#include "scenario\\scenario.h"
#include <optional>
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"
#include <variant>
#include <algorithm>
#include "effects.h"

namespace triggers {
	using effect_value = std::variant<std::monostate, int32_t, float, issues::option_identifier, trigger_payload>;

	struct capital_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::capital;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct add_core_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_positive_integer(t.start, t.end)) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return effect_codes::add_core_int;
				else
					return std::optional<uint16_t>();
			} else {
				if (scope.main_slot == trigger_slot_contents::province) {
					if (is_fixed_token_ci(t, "reb")) {
						if (scope.contains_rebeltype)
							return effect_codes::add_core_reb;
						else
							return std::optional<uint16_t>();
					} else if (is_fixed_token_ci(t, "from")) {
						if (scope.from_slot == trigger_slot_contents::nation)
							return effect_codes::add_core_from_nation;
						else if (scope.from_slot == trigger_slot_contents::province)
							return effect_codes::add_core_from_province;
						else
							return std::optional<uint16_t>();
					} else if (is_fixed_token_ci(t, "this")) {
						if (scope.this_slot == trigger_slot_contents::nation)
							return effect_codes::add_core_this_nation;
						else if (scope.this_slot == trigger_slot_contents::province)
							return effect_codes::add_core_this_province;
						else if (scope.this_slot == trigger_slot_contents::state)
							return effect_codes::add_core_this_state;
						else if (scope.this_slot == trigger_slot_contents::pop)
							return effect_codes::add_core_this_pop;
						else
							return std::optional<uint16_t>();
					} else {
						return effect_codes::add_core_tag;
					}
				} else {
					return std::optional<uint16_t>();
				}
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			if(is_positive_integer(t.start, t.end))
				return trigger_payload(token_to<uint16_t>(t));
			else
				return trigger_payload(
					tag_from_text(
						s.culutre_m.national_tags_index,
						cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct remove_core_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_positive_integer(t.start, t.end)) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return effect_codes::remove_core_int;
				else
					return std::optional<uint16_t>();
			} else {
				if (scope.main_slot == trigger_slot_contents::province) {
					if (is_fixed_token_ci(t, "reb")) {
						if (scope.contains_rebeltype)
							return effect_codes::remove_core_reb;
						else
							return std::optional<uint16_t>();
					} else if (is_fixed_token_ci(t, "from")) {
						if (scope.from_slot == trigger_slot_contents::nation)
							return effect_codes::remove_core_from_nation;
						else if (scope.from_slot == trigger_slot_contents::province)
							return effect_codes::remove_core_from_province;
						else
							return std::optional<uint16_t>();
					} else if (is_fixed_token_ci(t, "this")) {
						if (scope.this_slot == trigger_slot_contents::nation)
							return effect_codes::remove_core_this_nation;
						else if (scope.this_slot == trigger_slot_contents::province)
							return effect_codes::remove_core_this_province;
						else if (scope.this_slot == trigger_slot_contents::state)
							return effect_codes::remove_core_this_state;
						else if (scope.this_slot == trigger_slot_contents::pop)
							return effect_codes::remove_core_this_pop;
						else
							return std::optional<uint16_t>();
					} else {
						return effect_codes::remove_core_tag;
					}
				} else {
					return std::optional<uint16_t>();
				}
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			if (is_positive_integer(t.start, t.end))
				return trigger_payload(token_to<uint16_t>(t));
			else
				return trigger_payload(
					tag_from_text(
						s.culutre_m.national_tags_index,
						cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct change_region_name_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return effect_codes::change_region_name_state;
			else if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::change_region_name_province;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct trade_goods_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::trade_goods;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct add_accepted_culture_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "union"))
					return effect_codes::add_accepted_culture_union;
				else
					return effect_codes::add_accepted_culture;
			} else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct primary_culture_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::primary_culture_from_nation;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::primary_culture_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::primary_culture_this_province;
					else if (scope.this_slot == trigger_slot_contents::state)
						return effect_codes::primary_culture_this_state;
					else if (scope.this_slot == trigger_slot_contents::pop)
						return effect_codes::primary_culture_this_pop;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::primary_culture;
				}
				
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct remove_accepted_culture_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				return effect_codes::remove_accepted_culture;
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct life_rating_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::life_rating;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<int16_t>(t));
		}
	};
	struct religion_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::religion;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct is_slave_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes")) {
				if (scope.main_slot == trigger_slot_contents::state)
					return effect_codes::is_slave_state_yes;
				else if (scope.main_slot == trigger_slot_contents::pop)
					return effect_codes::is_slave_pop_yes;
				else
					return std::optional<uint16_t>();
			} else if (is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::state)
					return effect_codes::is_slave_state_no;
				else if (scope.main_slot == trigger_slot_contents::pop)
					return effect_codes::is_slave_pop_no;
				else
					return std::optional<uint16_t>();
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct research_points_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::research_points;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return token_to<float>(t);
		}
	};
	struct tech_school_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::tech_school;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.technology_m.named_tech_school_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct government_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return effect_codes::government_reb;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::government;
				}
			} else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.governments_m.named_government_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct treasury_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::treasury;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return token_to<float>(t);
		}
	};
	struct war_exhaustion_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::war_exhaustion;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return token_to<float>(t);
		}
	};
	struct prestige_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::prestige;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return token_to<float>(t);
		}
	};
	struct change_tag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "culture"))
					return effect_codes::change_tag_culture;
				else
					return effect_codes::change_tag;
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct change_tag_no_core_switch_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "culture"))
					return effect_codes::change_tag_no_core_switch_culture;
				else
					return effect_codes::change_tag_no_core_switch;
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct set_country_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::set_country_flag;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_payload(
				s.variables_m.get_named_national_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct clr_country_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::clr_country_flag;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_payload(
				s.variables_m.get_named_national_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)));
		}
	};
}