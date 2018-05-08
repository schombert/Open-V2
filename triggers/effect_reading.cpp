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
#include "events\\events.h"

namespace triggers {
	using effect_value = std::variant<std::monostate, int32_t, float, issues::option_identifier, trigger_payload>;

	struct capital_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::capital;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				s.variables_m.get_named_national_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct country_event_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::country_event_immediate;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state& scope, events::event_creation_manager& ecm) {
			return trigger_payload(
				ecm.register_triggered_event(
					s.event_m,
					token_to<int32_t>(t),
					trigger_scope_state{trigger_slot_contents::nation, trigger_slot_contents::nation, scope.this_slot, false}));
		}
	};
	struct province_event_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::province_event_immediate;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state& scope, events::event_creation_manager& ecm) {
			return trigger_payload(
				ecm.register_triggered_event(
					s.event_m,
					token_to<int32_t>(t),
					trigger_scope_state{ trigger_slot_contents::province, trigger_slot_contents::province, scope.this_slot, false }));
		}
	};
	struct military_access_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::military_access_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::military_access_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if(scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::military_access_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::military_access_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::military_access;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct badboy_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::badboy;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct secede_province_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::secede_province_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::secede_province_this_province;
					else if (scope.this_slot == trigger_slot_contents::state)
						return effect_codes::secede_province_this_state;
					else if (scope.this_slot == trigger_slot_contents::pop)
						return effect_codes::secede_province_this_pop;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::secede_province_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::secede_province_from_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "reb")) {
					if(scope.contains_rebeltype)
						return effect_codes::secede_province_reb;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::secede_province;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct inherit_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::inherit_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::inherit_this_province;
					else if (scope.this_slot == trigger_slot_contents::state)
						return effect_codes::inherit_this_state;
					else if (scope.this_slot == trigger_slot_contents::pop)
						return effect_codes::inherit_this_pop;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::inherit_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::inherit_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::inherit;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct annex_to_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::annex_to_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::annex_to_this_province;
					else if (scope.this_slot == trigger_slot_contents::state)
						return effect_codes::annex_to_this_state;
					else if (scope.this_slot == trigger_slot_contents::pop)
						return effect_codes::annex_to_this_pop;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::annex_to_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::annex_to_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::annex_to;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct release_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::release_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::release_this_province;
					else if (scope.this_slot == trigger_slot_contents::state)
						return effect_codes::release_this_state;
					else if (scope.this_slot == trigger_slot_contents::pop)
						return effect_codes::release_this_pop;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::release_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::release_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::release;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct change_controller_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::change_controller_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::change_controller_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::change_controller_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::change_controller_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::change_controller;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct infrastructure_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::infrastructure;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct money_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::money;
			else if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::treasury;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct leadership_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::leadership;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct create_vassal_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::create_vassal_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::create_vassal_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::create_vassal_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::create_vassal_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::create_vassal;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct end_military_access_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::end_military_access_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::end_military_access_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::end_military_access_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::end_military_access_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::end_military_access;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct leave_alliance_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::leave_alliance_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::leave_alliance_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::leave_alliance_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::leave_alliance_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::leave_alliance;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct end_war_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::end_war_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::end_war_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::end_war_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::end_war_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::end_war;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct enable_ideology_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type&) {
			return effect_codes::enable_ideology;
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct ruling_party_ideology_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::ruling_party_ideology;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct plurality_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::plurality;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct remove_province_modifier_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::remove_province_modifier;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct remove_country_modifier_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::remove_country_modifier;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct create_alliance_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::create_alliance_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::create_alliance_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::create_alliance_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::create_alliance_from_province;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::create_alliance;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct release_vassal_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return effect_codes::release_vassal_this_nation;
					else if (scope.this_slot == trigger_slot_contents::province)
						return effect_codes::release_vassal_this_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return effect_codes::release_vassal_from_nation;
					else if (scope.from_slot == trigger_slot_contents::province)
						return effect_codes::release_vassal_from_province;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return effect_codes::release_vassal_reb;
					else
						return std::optional<uint16_t>();
				} else if (is_fixed_token_ci(t, "random")) {
					return effect_codes::release_vassal_random;
				} else {
					return effect_codes::release_vassal;
				}
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct change_province_name_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::change_province_name;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
					text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct enable_canal_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			return effect_codes::enable_canal;
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<uint16_t>(t);
		}
	};
	struct set_global_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			return effect_codes::set_global_flag;
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload{
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct clr_global_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			return effect_codes::clr_global_flag;
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload{
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct nationalvalue_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::nationalvalue_nation;
			else if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::nationalvalue_province;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct civilized_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "yes"))
					return effect_codes::civilized_yes;
				else if (is_fixed_token_ci(t, "no"))
					return effect_codes::civilized_no;
				else
					return std::optional<uint16_t>();
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
}