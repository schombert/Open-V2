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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type&) {
			return effect_codes::enable_canal;
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct set_global_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type&) {
			return effect_codes::set_global_flag;
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload{
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct clr_global_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type&) {
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
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
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
	struct election_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::election;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct social_reform_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::social_reform;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct political_reform_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::political_reform;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct add_tax_relative_income_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::add_tax_relative_income;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct neutrality_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::neutrality;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct reduce_pop_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::reduce_pop;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct move_pop_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::move_pop;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct pop_type_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::pop_type;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.population_m.named_pop_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct years_of_research_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::years_of_research;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct prestige_factor_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				if(token_to<float>(t) >= 0.0f)
					return effect_codes::prestige_factor_positive;
				else
					return effect_codes::prestige_factor_negative;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct military_reform_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::military_reform;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct economic_reform_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::economic_reform;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct remove_random_military_reforms_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::remove_random_military_reforms;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct remove_random_economic_reforms_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::remove_random_economic_reforms;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<uint16_t>(t));
		}
	};
	struct add_crime_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "none"))
					return effect_codes::add_crime_none;
				else
					return effect_codes::add_crime;
			} else {
				return std::optional<uint16_t>();
			}
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct nationalize_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::nationalize;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct build_factory_in_capital_state_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::build_factory_in_capital_state;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_factory_types_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct activate_technology_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::activate_technology;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.technology_m.named_technology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct great_wars_enabled_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes"))
				return effect_codes::great_wars_enabled_yes;
			else if (is_fixed_token_ci(t, "no"))
				return effect_codes::great_wars_enabled_no;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct world_wars_enabled_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes"))
				return effect_codes::world_wars_enabled_yes;
			else if (is_fixed_token_ci(t, "no"))
				return effect_codes::world_wars_enabled_no;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct assimilate_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::assimilate_province;
			else if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::assimilate_pop;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct literacy_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::literacy;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct add_crisis_interest_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::add_crisis_interest;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<bool>(t));
		}
	};
	struct flashpoint_tension_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return effect_codes::flashpoint_tension;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct add_crisis_temperature_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type&) {
			return effect_codes::add_crisis_temperature;
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct consciousness_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::consciousness;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct militancy_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::militancy;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return token_to<float>(t);
		}
	};
	struct rgo_size_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::rgo_size;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<int16_t>(t));
		}
	};
	struct add_province_modifier_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::add_province_modifier_no_duration;
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
	struct add_country_modifier_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::add_country_modifier_no_duration;
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

#ifdef _DEBUG
#define EFFECT_ERROR(type, environment) throw type ();
#else
#define EFFECT_ERROR(type, environment) std::abort();
#endif

#ifdef _DEBUG
	struct trigger_revolt_missing_type {};
	struct invalid_scope_for_effect {};
#endif

	//complex
	struct trigger_revolt_effect {
		effect_parsing_environment& env;

		cultures::culture_tag culture;
		cultures::religion_tag religion;
		ideologies::ideology_tag ideology;
		population::rebel_type_tag type;

		bool this_v = false;

		trigger_revolt_effect(effect_parsing_environment& e) : env(e) {}

		void set_culture(const token_and_type& t) {
			culture = tag_from_text(
				env.s.culutre_m.named_culture_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_religion(const token_and_type& t) {
			religion = tag_from_text(
				env.s.culutre_m.named_religion_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.population_m.named_rebel_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (!is_valid_index(type))
				EFFECT_ERROR(trigger_revolt_missing_type, env);
			if (env.current_scope.main_slot == trigger_slot_contents::nation)
				env.data.push_back(effect_codes::trigger_revolt_nation);
			else if (env.current_scope.main_slot == trigger_slot_contents::state)
				env.data.push_back(effect_codes::trigger_revolt_state);
			else if (env.current_scope.main_slot == trigger_slot_contents::province)
				env.data.push_back(effect_codes::trigger_revolt_province);
			else
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(5ui16);
			env.data.push_back(trigger_payload(type).value);
			env.data.push_back(trigger_payload(culture).value);
			env.data.push_back(trigger_payload(religion).value);
			env.data.push_back(trigger_payload(ideology).value);

		}
	};

	struct diplomatic_influence_effect {
		effect_parsing_environment& env;

		int16_t value = 0;
		cultures::national_tag who_tag;

		bool who_from = false;
		bool who_this = false;

		diplomatic_influence_effect(effect_parsing_environment& e) : env(e) {}

		void set_who(const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				who_from = true;
			} else if (is_fixed_token_ci(t, "this")) {
				who_this = true;
			} else {
				who_tag = tag_from_text(
					env.s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if(env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);
			if (who_from) {
				if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::diplomatic_influence_from_nation);
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::diplomatic_influence_from_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else if (who_this) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::diplomatic_influence_this_nation);
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::diplomatic_influence_this_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else {
				env.data.push_back(effect_codes::diplomatic_influence);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(who_tag).value);
				env.data.push_back(trigger_payload(value).value);
			}
		}
	};

	struct relation_effect {
		effect_parsing_environment& env;

		int16_t value = 0;
		cultures::national_tag who_tag;

		bool who_from = false;
		bool who_this = false;
		bool who_reb = false;

		relation_effect(effect_parsing_environment& e) : env(e) {}

		void set_who(const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				who_from = true;
			} else if (is_fixed_token_ci(t, "this")) {
				who_this = true;
			} else if (is_fixed_token_ci(t, "reb")) {
				who_reb = true;
			} else {
				who_tag = tag_from_text(
					env.s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);
			if (who_from) {
				if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::relation_from_nation);
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::relation_from_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else if (who_this) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::relation_this_nation);
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::relation_this_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else if (who_reb) {
				if (!env.current_scope.contains_rebeltype)
					EFFECT_ERROR(invalid_scope_for_effect, env);
				env.data.push_back(effect_codes::relation_reb);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(value).value);
			} else {
				env.data.push_back(effect_codes::relation);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(who_tag).value);
				env.data.push_back(trigger_payload(value).value);
			}
		}
	};

	struct add_province_modifier_complex_effect {
		effect_parsing_environment& env;

		int16_t value = 0;
		modifiers::provincial_modifier_tag name;

		add_province_modifier_complex_effect(effect_parsing_environment& e) : env(e) {}

		void set_name(const token_and_type& t) {
			name = tag_from_text(
				env.s.modifiers_m.named_provincial_modifiers_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::province)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (value <= 0) {
				env.data.push_back(effect_codes::add_province_modifier_no_duration);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(name).value);
			} else {
				env.data.push_back(effect_codes::add_province_modifier);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(name).value);
				env.data.push_back(trigger_payload(value).value);
			}
		}
	};

	struct add_country_modifier_complex_effect {
		effect_parsing_environment& env;

		int16_t value = 0;
		modifiers::national_modifier_tag name;

		add_country_modifier_complex_effect(effect_parsing_environment& e) : env(e) {}

		void set_name(const token_and_type& t) {
			name = tag_from_text(
				env.s.modifiers_m.named_national_modifiers_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (value <= 0) {
				env.data.push_back(effect_codes::add_country_modifier_no_duration);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(name).value);
			} else {
				env.data.push_back(effect_codes::add_country_modifier);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(name).value);
				env.data.push_back(trigger_payload(value).value);
			}
		}
	};
	struct casus_belli_effect {
		effect_parsing_environment& env;

		int16_t months = 0;
		uint16_t province_target = 0;
		bool from_target = false;
		bool this_target = false;
		cultures::national_tag target_tag;
		military::cb_type_tag type;

		modifiers::national_modifier_tag name;

		casus_belli_effect(effect_parsing_environment& e) : env(e) {}

		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.military_m.named_cb_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_target(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				this_target = true;
			} else if (is_fixed_token_ci(t, "from")) {
				from_target = true;
			} else if (is_positive_integer(t.start, t.end)) {
				province_target = token_to<uint16_t>(t);
			} else {
				target_tag = tag_from_text(
					env.s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (from_target) {
				if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::casus_belli_from_nation);
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::casus_belli_from_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if (this_target) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::casus_belli_this_nation);
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::casus_belli_this_province);
				else if (env.current_scope.this_slot == trigger_slot_contents::state)
					env.data.push_back(effect_codes::casus_belli_this_state);
				else if (env.current_scope.this_slot == trigger_slot_contents::pop)
					env.data.push_back(effect_codes::casus_belli_this_pop);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if(is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::casus_belli_tag);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::casus_belli_int);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(province_target).value);
			}
		}
	};
	struct add_casus_belli_effect {
		effect_parsing_environment& env;

		int16_t months = 0;
		uint16_t province_target = 0;
		bool from_target = false;
		bool this_target = false;
		cultures::national_tag target_tag;
		military::cb_type_tag type;

		modifiers::national_modifier_tag name;

		add_casus_belli_effect(effect_parsing_environment& e) : env(e) {}

		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.military_m.named_cb_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_target(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				this_target = true;
			} else if (is_fixed_token_ci(t, "from")) {
				from_target = true;
			} else if (is_positive_integer(t.start, t.end)) {
				province_target = token_to<uint16_t>(t);
			} else {
				target_tag = tag_from_text(
					env.s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (from_target) {
				if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::add_casus_belli_from_nation);
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::add_casus_belli_from_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if (this_target) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::add_casus_belli_this_nation);
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::add_casus_belli_this_province);
				else if (env.current_scope.this_slot == trigger_slot_contents::state)
					env.data.push_back(effect_codes::add_casus_belli_this_state);
				else if (env.current_scope.this_slot == trigger_slot_contents::pop)
					env.data.push_back(effect_codes::add_casus_belli_this_pop);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if (is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::add_casus_belli_tag);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::add_casus_belli_int);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(province_target).value);
			}
		}
	};
	struct remove_casus_belli_effect {
		effect_parsing_environment& env;

		int16_t months = 0;
		uint16_t province_target = 0;
		bool from_target = false;
		bool this_target = false;
		cultures::national_tag target_tag;
		military::cb_type_tag type;

		modifiers::national_modifier_tag name;

		remove_casus_belli_effect(effect_parsing_environment& e) : env(e) {}

		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.military_m.named_cb_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_target(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				this_target = true;
			} else if (is_fixed_token_ci(t, "from")) {
				from_target = true;
			} else if (is_positive_integer(t.start, t.end)) {
				province_target = token_to<uint16_t>(t);
			} else {
				target_tag = tag_from_text(
					env.s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (from_target) {
				if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::remove_casus_belli_from_nation);
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::remove_casus_belli_from_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if (this_target) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::remove_casus_belli_this_nation);
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::remove_casus_belli_this_province);
				else if (env.current_scope.this_slot == trigger_slot_contents::state)
					env.data.push_back(effect_codes::remove_casus_belli_this_state);
				else if (env.current_scope.this_slot == trigger_slot_contents::pop)
					env.data.push_back(effect_codes::remove_casus_belli_this_pop);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if (is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::remove_casus_belli_tag);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::remove_casus_belli_int);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(province_target).value);
			}
		}
	};
	struct this_remove_casus_belli_effect {
		effect_parsing_environment& env;

		int16_t months = 0;
		uint16_t province_target = 0;
		bool from_target = false;
		bool this_target = false;
		cultures::national_tag target_tag;
		military::cb_type_tag type;

		modifiers::national_modifier_tag name;

		this_remove_casus_belli_effect(effect_parsing_environment& e) : env(e) {}

		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.military_m.named_cb_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_target(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				this_target = true;
			} else if (is_fixed_token_ci(t, "from")) {
				from_target = true;
			} else if (is_positive_integer(t.start, t.end)) {
				province_target = token_to<uint16_t>(t);
			} else {
				target_tag = tag_from_text(
					env.s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (from_target) {
				if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::this_remove_casus_belli_from_nation);
				else if (env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::this_remove_casus_belli_from_province);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if (this_target) {
				if (env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::this_remove_casus_belli_this_nation);
				else if (env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::this_remove_casus_belli_this_province);
				else if (env.current_scope.this_slot == trigger_slot_contents::state)
					env.data.push_back(effect_codes::this_remove_casus_belli_this_state);
				else if (env.current_scope.this_slot == trigger_slot_contents::pop)
					env.data.push_back(effect_codes::this_remove_casus_belli_this_pop);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(months).value);
			} else if (is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::this_remove_casus_belli_tag);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::this_remove_casus_belli_int);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(province_target).value);
			}
		}
	};

	struct limited_wg_reader {
		effect_parsing_environment& env;


		cultures::national_tag country;
		uint16_t state_province_id = 0;
		military::cb_type_tag casus_belli;

		limited_wg_reader(effect_parsing_environment& e) : env(e) {}

		void set_country(const token_and_type& t) {
			country = tag_from_text(
				env.s.culutre_m.national_tags_index,
				cultures::tag_to_encoding(t.start, t.end));
		}
		void set_casus_belli(const token_and_type& t) {
			casus_belli = tag_from_text(
				env.s.military_m.named_cb_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
	};

	struct war_effect {
		effect_parsing_environment& env;

		std::vector<limited_wg_reader> attacker_goals;
		std::vector<limited_wg_reader> defender_goals;
		cultures::national_tag target_tag;
		bool from_target = false;
		bool this_target = false;
		bool call_ally = false;

		modifiers::national_modifier_tag name;

		war_effect(effect_parsing_environment& e) : env(e) {}

		void add_attacker_wargoal(const limited_wg_reader& t) {
			attacker_goals.emplace_back(t);
		}
		void add_defender_wargoal(const limited_wg_reader& t) {
			defender_goals.emplace_back(t);
		}
		void set_target(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				this_target = true;
			} else if (is_fixed_token_ci(t, "from")) {
				from_target = true;
			} else {
				target_tag = tag_from_text(
					env.s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (call_ally) {
				if (from_target) {
					if (env.current_scope.from_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::war_from_nation);
					else if (env.current_scope.from_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::war_from_province);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);
				} else if (this_target) {
					if (env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::war_this_nation);
					else if (env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::war_this_province);
					else if (env.current_scope.this_slot == trigger_slot_contents::state)
						env.data.push_back(effect_codes::war_this_state);
					else if (env.current_scope.this_slot == trigger_slot_contents::pop)
						env.data.push_back(effect_codes::war_this_pop);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);
				} else {
					env.data.push_back(effect_codes::war_tag);
				}
			} else {
				if (from_target) {
					if (env.current_scope.from_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::war_no_ally_from_nation);
					else if (env.current_scope.from_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::war_no_ally_from_province);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);
				} else if (this_target) {
					if (env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::war_no_ally_this_nation);
					else if (env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::war_no_ally_this_province);
					else if (env.current_scope.this_slot == trigger_slot_contents::state)
						env.data.push_back(effect_codes::war_no_ally_this_state);
					else if (env.current_scope.this_slot == trigger_slot_contents::pop)
						env.data.push_back(effect_codes::war_no_ally_this_pop);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);
				} else {
					env.data.push_back(effect_codes::war_no_ally_tag);
				}
			}

			env.data.push_back(uint16_t(3 + 3 * attacker_goals.size() + 3 * defender_goals.size()));
			env.data.push_back(uint16_t(attacker_goals.size()));
			for (const auto& wg : attacker_goals) {
				env.data.push_back(trigger_payload(wg.casus_belli).value);
				env.data.push_back(trigger_payload(wg.state_province_id).value);
				env.data.push_back(trigger_payload(wg.country).value);
			}
			env.data.push_back(uint16_t(defender_goals.size()));
			for (const auto& wg : defender_goals) {
				env.data.push_back(trigger_payload(wg.casus_belli).value);
				env.data.push_back(trigger_payload(wg.state_province_id).value);
				env.data.push_back(trigger_payload(wg.country).value);
			}
		}
	};

	struct country_event_complex_effect {
		effect_parsing_environment& env;

		int16_t days = 0;
		int32_t id = 0;

		country_event_complex_effect(effect_parsing_environment& e) : env(e) {}

		void finalize() const {
			if (days <= 0) {
				env.data.push_back(effect_codes::country_event_immediate);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(
					env.ecm.register_triggered_event(
						env.s.event_m,
						id,
						trigger_scope_state{ trigger_slot_contents::nation, trigger_slot_contents::nation, env.current_scope.this_slot, false })).value);
			} else {
				env.data.push_back(effect_codes::country_event);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(
					env.ecm.register_triggered_event(
						env.s.event_m,
						id,
						trigger_scope_state{ trigger_slot_contents::nation, trigger_slot_contents::nation, env.current_scope.this_slot, false })).value);
				env.data.push_back(uint16_t(days));
			}
		}
	};
	struct province_event_complex_effect {
		effect_parsing_environment& env;

		int16_t days = 0;
		int32_t id = 0;

		province_event_complex_effect(effect_parsing_environment& e) : env(e) {}

		void finalize() const {
			if (days <= 0) {
				env.data.push_back(effect_codes::province_event_immediate);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(
					env.ecm.register_triggered_event(
						env.s.event_m,
						id,
						trigger_scope_state{ trigger_slot_contents::province, trigger_slot_contents::province, env.current_scope.this_slot, false })).value);
			} else {
				env.data.push_back(effect_codes::province_event);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(
					env.ecm.register_triggered_event(
						env.s.event_m,
						id,
						trigger_scope_state{ trigger_slot_contents::province, trigger_slot_contents::province, env.current_scope.this_slot, false })).value);
				env.data.push_back(uint16_t(days));
			}
		}
	};

	struct sub_unit_effect {
		effect_parsing_environment& env;

		uint16_t value = 0;
		bool value_this = false;
		bool value_from = false;
		bool value_current = false;
		military::unit_type_tag type;

		sub_unit_effect(effect_parsing_environment& e) : env(e) {}

		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.military_m.named_unit_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_value(const token_and_type& t) {
			if (is_fixed_token_ci(t, "this"))
				value_this = true;
			else if (is_fixed_token_ci(t, "from"))
				value_from = true;
			else if (is_fixed_token_ci(t, "current"))
				value_current = true;
			else
				value = token_to<uint16_t>(t);
		}

		void finalize() const {
			if (value_this) {
				if (env.current_scope.main_slot != trigger_slot_contents::nation || env.current_scope.this_slot != trigger_slot_contents::province)
					EFFECT_ERROR(invalid_scope_for_effect, env);
				env.data.push_back(effect_codes::sub_unit_this);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(type).value);
			} else if (value_from) {
				if (env.current_scope.main_slot != trigger_slot_contents::nation || env.current_scope.from_slot != trigger_slot_contents::province)
					EFFECT_ERROR(invalid_scope_for_effect, env);
				env.data.push_back(effect_codes::sub_unit_from);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(type).value);
			} else if (value_current) {
				if (env.current_scope.main_slot != trigger_slot_contents::province)
					EFFECT_ERROR(invalid_scope_for_effect, env);
				env.data.push_back(effect_codes::sub_unit_current);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(type).value);
			} else {
				if (env.current_scope.main_slot != trigger_slot_contents::nation)
					EFFECT_ERROR(invalid_scope_for_effect, env);
				env.data.push_back(effect_codes::sub_unit_int);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(value);
			}
		}
	};

	struct set_variable_effect {
		effect_parsing_environment& env;

		float value = 0.0f;
		variables::national_variable_tag which;

		set_variable_effect(effect_parsing_environment& e) : env(e) {}

		void set_which(const token_and_type& t) {
			which = tag_from_text(
				env.s.variables_m.named_national_variables,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::set_variable);
			env.data.push_back(4ui16);
			env.data.push_back(trigger_payload(which).value);
			add_float_to_payload(env.data, value);
		}
	};

	struct change_variable_effect {
		effect_parsing_environment& env;

		float value = 0.0f;
		variables::national_variable_tag which;

		change_variable_effect(effect_parsing_environment& e) : env(e) {}

		void set_which(const token_and_type& t) {
			which = tag_from_text(
				env.s.variables_m.named_national_variables,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::change_variable);
			env.data.push_back(4ui16);
			env.data.push_back(trigger_payload(which).value);
			add_float_to_payload(env.data, value);
		}
	};

	struct ideology_effect {
		effect_parsing_environment& env;

		float factor = 0.0f;
		ideologies::ideology_tag value;

		ideology_effect(effect_parsing_environment& e) : env(e) {}

		void set_value(const token_and_type& t) {
			value = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::pop)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::ideology);
			env.data.push_back(4ui16);
			env.data.push_back(trigger_payload(value).value);
			add_float_to_payload(env.data, factor);
		}
	};
	struct dominant_issue_effect {
		effect_parsing_environment& env;

		float factor = 0.0f;
		issues::option_identifier value;

		dominant_issue_effect(effect_parsing_environment& e) : env(e) {}

		void set_value(const token_and_type& t) {
			value = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::pop)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::dominant_issue);
			env.data.push_back(5ui16);
			add_option_identifier_to_payload(env.data, value);
			add_float_to_payload(env.data, factor);
		}
	};
	struct upper_house_effect {
		effect_parsing_environment& env;

		float value = 0.0f;
		ideologies::ideology_tag ideology;

		upper_house_effect(effect_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::upper_house);
			env.data.push_back(4ui16);
			env.data.push_back(trigger_payload(ideology).value);
			add_float_to_payload(env.data, value);
		}
	};
	struct scaled_militancy_effect {
		effect_parsing_environment& env;

		float factor = 0.0f;
		float unemployment = 0.0f;
		ideologies::ideology_tag ideology;
		issues::option_identifier issue;

		scaled_militancy_effect(effect_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_issue(const token_and_type& t) {
			issue = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::pop)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (is_valid_index(ideology)) {
				env.data.push_back(effect_codes::scaled_militancy_ideology);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(ideology).value);
				add_float_to_payload(env.data, factor);
			} else if (is_valid_index(issue.type)) {
				env.data.push_back(effect_codes::scaled_militancy_issue);
				env.data.push_back(5ui16);
				add_option_identifier_to_payload(env.data, issue);
				add_float_to_payload(env.data, factor);
			} else {
				env.data.push_back(effect_codes::scaled_militancy_unemployment);
				env.data.push_back(3ui16);
				add_float_to_payload(env.data, factor * unemployment);
			}
		}
	};
	struct scaled_consciousness_effect {
		effect_parsing_environment& env;

		float factor = 0.0f;
		float unemployment = 0.0f;
		ideologies::ideology_tag ideology;
		issues::option_identifier issue;

		scaled_consciousness_effect(effect_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_issue(const token_and_type& t) {
			issue = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::pop)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (is_valid_index(ideology)) {
				env.data.push_back(effect_codes::scaled_consciousness_ideology);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(ideology).value);
				add_float_to_payload(env.data, factor);
			} else if (is_valid_index(issue.type)) {
				env.data.push_back(effect_codes::scaled_consciousness_issue);
				env.data.push_back(5ui16);
				add_option_identifier_to_payload(env.data, issue);
				add_float_to_payload(env.data, factor);
			} else {
				env.data.push_back(effect_codes::scaled_consciousness_unemployment);
				env.data.push_back(3ui16);
				add_float_to_payload(env.data, factor * unemployment);
			}
		}
	};

	struct define_general_effect {
		effect_parsing_environment& env;

		text_data::text_tag name;
		military::leader_trait_tag personality;
		military::leader_trait_tag background;

		define_general_effect(effect_parsing_environment& e) : env(e) {}

		void set_personality(const token_and_type& t) {
			personality = tag_from_text(
				env.s.military_m.named_leader_trait_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_background(const token_and_type& t) {
			background = tag_from_text(
				env.s.military_m.named_leader_trait_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_name(const token_and_type& t) {
			name = text_data::get_thread_safe_text_handle(env.s.text_m, t.start, t.end);
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::define_general);
			env.data.push_back(4ui16);
			env.data.push_back(trigger_payload(name).value);
			env.data.push_back(trigger_payload(personality).value);
			env.data.push_back(trigger_payload(background).value);
		}
	};
	struct define_admiral_effect {
		effect_parsing_environment& env;

		text_data::text_tag name;
		military::leader_trait_tag personality;
		military::leader_trait_tag background;

		define_admiral_effect(effect_parsing_environment& e) : env(e) {}

		void set_personality(const token_and_type& t) {
			personality = tag_from_text(
				env.s.military_m.named_leader_trait_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_background(const token_and_type& t) {
			background = tag_from_text(
				env.s.military_m.named_leader_trait_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_name(const token_and_type& t) {
			name = text_data::get_thread_safe_text_handle(env.s.text_m, t.start, t.end);
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::define_admiral);
			env.data.push_back(4ui16);
			env.data.push_back(trigger_payload(name).value);
			env.data.push_back(trigger_payload(personality).value);
			env.data.push_back(trigger_payload(background).value);
		}
	};
	struct add_war_goal_effect {
		effect_parsing_environment& env;

		military::cb_type_tag casus_belli;

		add_war_goal_effect(effect_parsing_environment& e) : env(e) {}

		void set_casus_belli(const token_and_type& t) {
			casus_belli = tag_from_text(
				env.s.military_m.named_cb_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation || env.current_scope.from_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::add_war_goal);
			env.data.push_back(2ui16);
			env.data.push_back(trigger_payload(casus_belli).value);
		}
	};
	struct move_issue_percentage_effect {
		effect_parsing_environment& env;

		float value = 0.0f;
		issues::option_identifier from;
		issues::option_identifier to;

		move_issue_percentage_effect(effect_parsing_environment& e) : env(e) {}

		void set_from(const token_and_type& t) {
			from = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}
		void set_to(const token_and_type& t) {
			to = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				env.data.push_back(effect_codes::move_issue_percentage_nation);
			else if (env.current_scope.main_slot != trigger_slot_contents::state)
				env.data.push_back(effect_codes::move_issue_percentage_state);
			else if(env.current_scope.main_slot != trigger_slot_contents::province)
				env.data.push_back(effect_codes::move_issue_percentage_province);
			else if (env.current_scope.main_slot != trigger_slot_contents::pop)
				env.data.push_back(effect_codes::move_issue_percentage_pop);
			else
				EFFECT_ERROR(invalid_scope_for_effect, env);
			
			env.data.push_back(7ui16);
			add_option_identifier_to_payload(env.data, from);
			add_option_identifier_to_payload(env.data, to);
			add_float_to_payload(env.data, value);
		}
	};
	struct party_loyalty_effect {
		effect_parsing_environment& env;

		float loyalty_value = 0.0f;
		ideologies::ideology_tag ideology;
		std::optional<uint16_t> province_id;

		party_loyalty_effect(effect_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.text_m, t.start, t.end));
		}

		void finalize() const {
			if (province_id) {
				if (env.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::party_loyalty_province_id_nation);
				else if (env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::party_loyalty_province_id_from_nation);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(5ui16);
				env.data.push_back(*province_id);
				env.data.push_back(trigger_payload(ideology).value);
				add_float_to_payload(env.data, loyalty_value);
			} else {
				if (env.current_scope.main_slot == trigger_slot_contents::nation && env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::party_loyalty_nation_from_province);
				else if (env.current_scope.main_slot == trigger_slot_contents::province && env.current_scope.from_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::party_loyalty_province_from_nation);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(ideology).value);
				add_float_to_payload(env.data, loyalty_value);
			}
		}
	};
	struct build_railway_in_capital_effect {
		effect_parsing_environment& env;

		bool in_whole_capital_state = false;
		bool limit_to_world_greatest_level = false;

		build_railway_in_capital_effect(effect_parsing_environment& e) : env(e) {}


		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if(in_whole_capital_state && limit_to_world_greatest_level)
				env.data.push_back(uint16_t(effect_codes::build_railway_in_capital_yes_whole_state_yes_limit | effect_codes::no_payload));
			else if (in_whole_capital_state && !limit_to_world_greatest_level)
				env.data.push_back(uint16_t(effect_codes::build_railway_in_capital_yes_whole_state_no_limit | effect_codes::no_payload));
			else if (!in_whole_capital_state && limit_to_world_greatest_level)
				env.data.push_back(uint16_t(effect_codes::build_railway_in_capital_no_whole_state_yes_limit | effect_codes::no_payload));
			else
				env.data.push_back(uint16_t(effect_codes::build_railway_in_capital_no_whole_state_no_limit | effect_codes::no_payload));
		}
	};
	struct build_fort_in_capital_effect {
		effect_parsing_environment& env;

		bool in_whole_capital_state = false;
		bool limit_to_world_greatest_level = false;

		build_fort_in_capital_effect(effect_parsing_environment& e) : env(e) {}


		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (in_whole_capital_state && limit_to_world_greatest_level)
				env.data.push_back(uint16_t(effect_codes::build_fort_in_capital_yes_whole_state_yes_limit | effect_codes::no_payload));
			else if (in_whole_capital_state && !limit_to_world_greatest_level)
				env.data.push_back(uint16_t(effect_codes::build_fort_in_capital_yes_whole_state_no_limit | effect_codes::no_payload));
			else if (!in_whole_capital_state && limit_to_world_greatest_level)
				env.data.push_back(uint16_t(effect_codes::build_fort_in_capital_no_whole_state_yes_limit | effect_codes::no_payload));
			else
				env.data.push_back(uint16_t(effect_codes::build_fort_in_capital_no_whole_state_no_limit | effect_codes::no_payload));
		}
	};
}

MEMBER_FDEF(triggers::trigger_revolt_effect, set_culture, "culture");
MEMBER_FDEF(triggers::trigger_revolt_effect, set_religion, "religion");
MEMBER_FDEF(triggers::trigger_revolt_effect, set_ideology, "ideology");
MEMBER_FDEF(triggers::trigger_revolt_effect, set_type, "type");
MEMBER_FDEF(triggers::diplomatic_influence_effect, set_who, "who");
MEMBER_DEF(triggers::diplomatic_influence_effect, value, "value");
MEMBER_FDEF(triggers::relation_effect, set_who, "who");
MEMBER_DEF(triggers::relation_effect, value, "value");
MEMBER_FDEF(triggers::add_province_modifier_complex_effect, set_name, "name");
MEMBER_DEF(triggers::add_province_modifier_complex_effect, value, "value");
MEMBER_FDEF(triggers::add_country_modifier_complex_effect, set_name, "name");
MEMBER_DEF(triggers::add_country_modifier_complex_effect, value, "value");
MEMBER_FDEF(triggers::casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::casus_belli_effect, set_target, "target");
MEMBER_DEF(triggers::casus_belli_effect, months, "months");
MEMBER_FDEF(triggers::add_casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::add_casus_belli_effect, set_target, "target");
MEMBER_DEF(triggers::add_casus_belli_effect, months, "months");
MEMBER_FDEF(triggers::remove_casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::remove_casus_belli_effect, set_target, "target");
MEMBER_DEF(triggers::remove_casus_belli_effect, months, "months");
MEMBER_FDEF(triggers::this_remove_casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::this_remove_casus_belli_effect, set_target, "target");
MEMBER_DEF(triggers::this_remove_casus_belli_effect, months, "months");
MEMBER_FDEF(triggers::limited_wg_reader, set_country, "country");
MEMBER_FDEF(triggers::limited_wg_reader, set_casus_belli, "casus_belli");
MEMBER_DEF(triggers::limited_wg_reader, state_province_id, "state_province_id");
MEMBER_FDEF(triggers::war_effect, add_attacker_wargoal, "attacker_goal");
MEMBER_FDEF(triggers::war_effect, add_defender_wargoal, "defender_goal");
MEMBER_FDEF(triggers::war_effect, set_target, "target");
MEMBER_DEF(triggers::war_effect, call_ally, "call_ally");
MEMBER_DEF(triggers::country_event_complex_effect, days, "days");
MEMBER_DEF(triggers::country_event_complex_effect, id, "id");
MEMBER_DEF(triggers::province_event_complex_effect, days, "days");
MEMBER_DEF(triggers::province_event_complex_effect, id, "id");
MEMBER_FDEF(triggers::sub_unit_effect, set_type, "type");
MEMBER_FDEF(triggers::sub_unit_effect, set_value, "value");
MEMBER_FDEF(triggers::set_variable_effect, set_which, "which");
MEMBER_DEF(triggers::set_variable_effect, value, "value");
MEMBER_FDEF(triggers::change_variable_effect, set_which, "which");
MEMBER_DEF(triggers::change_variable_effect, value, "value");
MEMBER_FDEF(triggers::ideology_effect, set_value, "value");
MEMBER_DEF(triggers::ideology_effect, factor, "factor");
MEMBER_FDEF(triggers::dominant_issue_effect, set_value, "value");
MEMBER_DEF(triggers::dominant_issue_effect, factor, "factor");
MEMBER_FDEF(triggers::upper_house_effect, set_ideology, "ideology");
MEMBER_DEF(triggers::upper_house_effect, value, "value");
MEMBER_DEF(triggers::scaled_militancy_effect, factor, "factor");
MEMBER_DEF(triggers::scaled_militancy_effect, unemployment, "unemployment");
MEMBER_FDEF(triggers::scaled_militancy_effect, set_issue, "issue");
MEMBER_FDEF(triggers::scaled_militancy_effect, set_ideology, "ideology");
MEMBER_DEF(triggers::scaled_consciousness_effect, factor, "factor");
MEMBER_DEF(triggers::scaled_consciousness_effect, unemployment, "unemployment");
MEMBER_FDEF(triggers::scaled_consciousness_effect, set_issue, "issue");
MEMBER_FDEF(triggers::scaled_consciousness_effect, set_ideology, "ideology");
MEMBER_FDEF(triggers::define_general_effect, set_name, "name");
MEMBER_FDEF(triggers::define_general_effect, set_background, "background");
MEMBER_FDEF(triggers::define_general_effect, set_personality, "personality");
MEMBER_FDEF(triggers::define_admiral_effect, set_name, "name");
MEMBER_FDEF(triggers::define_admiral_effect, set_background, "background");
MEMBER_FDEF(triggers::define_admiral_effect, set_personality, "personality");
MEMBER_FDEF(triggers::add_war_goal_effect, set_casus_belli, "casus_belli");
MEMBER_DEF(triggers::move_issue_percentage_effect, value, "value");
MEMBER_FDEF(triggers::move_issue_percentage_effect, set_from, "from");
MEMBER_FDEF(triggers::move_issue_percentage_effect, set_to, "to");
MEMBER_DEF(triggers::party_loyalty_effect, loyalty_value, "loyalty_value");
MEMBER_DEF(triggers::party_loyalty_effect, province_id, "province_id");
MEMBER_FDEF(triggers::party_loyalty_effect, set_ideology, "ideology");
MEMBER_DEF(triggers::build_railway_in_capital_effect, in_whole_capital_state, "in_whole_capital_state");
MEMBER_DEF(triggers::build_railway_in_capital_effect, limit_to_world_greatest_level, "limit_to_world_greatest_level");
MEMBER_DEF(triggers::build_fort_in_capital_effect, in_whole_capital_state, "in_whole_capital_state");
MEMBER_DEF(triggers::build_fort_in_capital_effect, limit_to_world_greatest_level, "limit_to_world_greatest_level");
