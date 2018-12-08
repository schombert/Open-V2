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
#include "trigger_reading.h"
#include "object_parsing\\object_parsing.hpp"
#include "events\\events_io.h"
#include "modifiers\\modifiers_io.h"

namespace triggers {
	using effect_value = std::variant<std::monostate, int32_t, float, trigger_payload>;

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
						if (scope.from_slot == trigger_slot_contents::rebel)
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
						s.culture_m.national_tags_index,
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
						if (scope.from_slot == trigger_slot_contents::rebel)
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
						s.culture_m.national_tags_index,
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
		}
	};
	struct trade_goods_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::trade_goods;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culture_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culture_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culture_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.culture_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
			return trigger_payload(token_to<int16_t>(t));
		}
	};
	struct tech_school_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::tech_school;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
		}
	};
	struct government_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "reb")) {
					if (scope.from_slot == trigger_slot_contents::rebel)
						return effect_codes::government_reb;
					else
						return std::optional<uint16_t>();
				} else {
					return effect_codes::government;
				}
			} else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.governments_m.named_government_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
		}
	};
	struct treasury_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::treasury;
			else if(scope.main_slot == trigger_slot_contents::province)
				return effect_codes::treasury_province;
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
			return token_to<float>(t) / 100.0f;
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct set_country_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::set_country_flag;
			else if(scope.main_slot == trigger_slot_contents::province)
				return effect_codes::set_country_flag_province;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				s.variables_m.get_named_national_flag(text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
				s.variables_m.get_named_national_flag(text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
		}
	};
	struct country_event_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if(scope.main_slot == trigger_slot_contents::nation) {
				if(scope.this_slot == trigger_slot_contents::nation)
					return effect_codes::country_event_immediate_this_nation;
				else if(scope.this_slot == trigger_slot_contents::province)
					return effect_codes::country_event_immediate_this_province;
				else if(scope.this_slot == trigger_slot_contents::state)
					return effect_codes::country_event_immediate_this_state;
				else if(scope.this_slot == trigger_slot_contents::pop)
					return effect_codes::country_event_immediate_this_pop;
				else
					return std::optional<uint16_t>();
			} else if(scope.main_slot == trigger_slot_contents::province) {
				if(scope.this_slot == trigger_slot_contents::nation)
					return effect_codes::country_event_immediate_province_this_nation;
				else if(scope.this_slot == trigger_slot_contents::province)
					return effect_codes::country_event_immediate_province_this_province;
				else if(scope.this_slot == trigger_slot_contents::state)
					return effect_codes::country_event_immediate_province_this_state;
				else if(scope.this_slot == trigger_slot_contents::pop)
					return effect_codes::country_event_immediate_province_this_pop;
				else
					return std::optional<uint16_t>();
			} else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state& scope, events::event_creation_manager& ecm) {
			return trigger_payload(
				ecm.register_triggered_event(
					s.event_m,
					token_to<int32_t>(t),
					trigger_scope_state{trigger_slot_contents::nation, trigger_slot_contents::nation, scope.this_slot}));
		}
	};
	struct province_event_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if(scope.main_slot == trigger_slot_contents::nation) {
				if(scope.this_slot == trigger_slot_contents::nation)
					return effect_codes::province_event_immediate_this_nation;
				else if(scope.this_slot == trigger_slot_contents::state)
					return effect_codes::province_event_immediate_this_state;
				else if(scope.this_slot == trigger_slot_contents::province)
					return effect_codes::province_event_immediate_this_province;
				else if(scope.this_slot == trigger_slot_contents::pop)
					return effect_codes::province_event_immediate_this_pop;
				else
					return std::optional<uint16_t>();
			} else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state& scope, events::event_creation_manager& ecm) {
			return trigger_payload(
				ecm.register_triggered_event(
					s.event_m,
					token_to<int32_t>(t),
					trigger_scope_state{ trigger_slot_contents::province, trigger_slot_contents::province, scope.this_slot }));
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
					s.culture_m.national_tags_index,
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
					if(scope.from_slot == trigger_slot_contents::rebel)
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
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
			return trigger_payload(token_to<int16_t>(t));
		}
	};
	struct fort_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::fort;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<int16_t>(t));
		}
	};
	struct naval_base_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::naval_base;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, const scenario::scenario_manager&, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(token_to<int16_t>(t));
		}
	};
	struct money_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return effect_codes::money;
			else if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::treasury;
			else if(scope.main_slot == trigger_slot_contents::province)
				return effect_codes::treasury_province;
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
			return trigger_payload(token_to<int16_t>(t));
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
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
					s.culture_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct enable_ideology_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type&) {
			return effect_codes::enable_ideology;
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
		}
	};
	struct ruling_party_ideology_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::ruling_party_ideology;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
			return token_to<float>(t) / 100.0f;
		}
	};
	struct remove_province_modifier_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return effect_codes::remove_province_modifier;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
		}
	};
	struct remove_country_modifier_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::remove_country_modifier;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
					s.culture_m.national_tags_index,
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
					if (scope.from_slot == trigger_slot_contents::rebel)
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
					s.culture_m.national_tags_index,
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
					text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
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
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, t.start, t.end)) };
		}
	};
	struct clr_global_flag_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&, association_type, const token_and_type&) {
			return effect_codes::clr_global_flag;
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload{
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, t.start, t.end)) };
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
		}
	};
	struct political_reform_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::political_reform;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.population_m.named_pop_type_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
		}
	};
	struct economic_reform_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope, association_type, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return effect_codes::economic_reform;
			else
				return std::optional<uint16_t>();
		}
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return tag_from_text(
				s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
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
		static effect_value read_value(const token_and_type& t, scenario::scenario_manager& s, const trigger_scope_state&, events::event_creation_manager&) {
			return trigger_payload(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
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
					text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end)));
		}
	};

	//scope effect
	struct generic_scope_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return uint16_t(effect_codes::generic_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return scope;
		}
	};
	struct any_neighbor_province_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::x_neighbor_province_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct any_neighbor_country_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_neighbor_country_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct any_country_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return uint16_t(effect_codes::x_country_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct random_country_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_country_scope_nation | effect_codes::is_random_scope);
			else
				return uint16_t(effect_codes::x_country_scope | effect_codes::is_random_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct random_neighbor_province_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::x_neighbor_province_scope | effect_codes::is_random_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct random_empty_neighbor_province_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::x_empty_neighbor_province_scope | effect_codes::is_random_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct any_greater_power_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
				return uint16_t(effect_codes::x_greater_power_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct poor_strata_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::poor_strata_scope_nation);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::poor_strata_scope_state);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::poor_strata_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::pop,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct middle_strata_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::middle_strata_scope_nation);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::middle_strata_scope_state);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::middle_strata_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::pop,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct rich_strata_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::rich_strata_scope_nation);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::rich_strata_scope_state);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::rich_strata_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::pop,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct random_pop_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_pop_scope_nation | effect_codes::is_random_scope);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::x_pop_scope_state | effect_codes::is_random_scope);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::x_pop_scope_province | effect_codes::is_random_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::pop,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct random_owned_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_owned_scope_nation | effect_codes::is_random_scope);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::x_owned_scope_state | effect_codes::is_random_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct any_owned_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_owned_scope_nation);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::x_owned_scope_state);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct all_core_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_core_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct any_state_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_state_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::state,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct random_state_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_state_scope | effect_codes::is_random_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::state,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct any_pop_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::x_pop_scope_nation);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::x_pop_scope_state);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::x_pop_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::pop,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct owner_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::owner_scope_state);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::owner_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct controller_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::controller_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct location_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(effect_codes::location_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct country_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(effect_codes::country_scope_pop);
			else if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::country_scope_state);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct capital_scope_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::capital_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct this_scope_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.this_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::this_scope_nation);
			else if (scope.this_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::this_scope_province);
			else if (scope.this_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::this_scope_state);
			else if (scope.this_slot == trigger_slot_contents::pop)
				return uint16_t(effect_codes::this_scope_pop);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				scope.this_slot,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct from_scope_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.from_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::from_scope_nation);
			else if (scope.from_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::from_scope_province);
			else if (scope.from_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::from_scope_state);
			else if (scope.from_slot == trigger_slot_contents::pop)
				return uint16_t(effect_codes::from_scope_pop);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				scope.from_slot,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct sea_zone_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::sea_zone_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::province,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct cultural_union_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::cultural_union_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct overlord_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::overlord_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct sphere_owner_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return uint16_t(effect_codes::sphere_owner_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct independence_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.from_slot == trigger_slot_contents::rebel)
				return uint16_t(effect_codes::independence_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct flashpoint_tag_scope_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::state)
				return uint16_t(effect_codes::flashpoint_tag_scope);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::nation,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct crisis_state_scope_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state&) {
			return uint16_t(effect_codes::crisis_state_scope);
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::state,
				scope.this_slot,
				scope.from_slot,
				 };
		}
	};
	struct state_scope_effect {
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return uint16_t(effect_codes::state_scope_pop);
			else if (scope.main_slot == trigger_slot_contents::province)
				return uint16_t(effect_codes::state_scope_province);
			else
				return std::optional<uint16_t>();
		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {
			return trigger_scope_state{
				trigger_slot_contents::state,
				scope.this_slot,
				scope.from_slot,
				 };
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
				env.s.culture_m.named_culture_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_religion(const token_and_type& t) {
			religion = tag_from_text(
				env.s.culture_m.named_religion_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_type(const token_and_type& t) {
			type = tag_from_text(
				env.s.population_m.named_rebel_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
					env.s.culture_m.national_tags_index,
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
					env.s.culture_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if(env.current_scope.main_slot == trigger_slot_contents::nation) {
				if(who_from) {
					if(env.current_scope.from_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::relation_from_nation);
					else if(env.current_scope.from_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::relation_from_province);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(value).value);
				} else if(who_this) {
					if(env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::relation_this_nation);
					else if(env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::relation_this_province);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(value).value);
				} else if(who_reb) {
					if(env.current_scope.from_slot != trigger_slot_contents::rebel)
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
			} else if(env.current_scope.main_slot == trigger_slot_contents::province) {
				if(who_from) {
					if(env.current_scope.from_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::relation_province_from_nation);
					else if(env.current_scope.from_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::relation_province_from_province);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(value).value);
				} else if(who_this) {
					if(env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::relation_province_this_nation);
					else if(env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::relation_province_this_province);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(value).value);
				} else if(who_reb) {
					if(env.current_scope.from_slot != trigger_slot_contents::rebel)
						EFFECT_ERROR(invalid_scope_for_effect, env);
					env.data.push_back(effect_codes::relation_province_reb);
					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(value).value);
				} else {
					env.data.push_back(effect_codes::relation_province);
					env.data.push_back(3ui16);
					env.data.push_back(trigger_payload(who_tag).value);
					env.data.push_back(trigger_payload(value).value);
				}
			} else {
				EFFECT_ERROR(invalid_scope_for_effect, env);
			}
		}
	};

	struct add_province_modifier_complex_effect {
		effect_parsing_environment& env;

		int16_t duration = 0;
		modifiers::provincial_modifier_tag name;

		add_province_modifier_complex_effect(effect_parsing_environment& e) : env(e) {}

		void set_name(const token_and_type& t) {
			name = tag_from_text(
				env.s.modifiers_m.named_provincial_modifiers_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::province)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (duration <= 0) {
				env.data.push_back(effect_codes::add_province_modifier_no_duration);
				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(name).value);
			} else {
				env.data.push_back(effect_codes::add_province_modifier);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(name).value);
				env.data.push_back(trigger_payload(duration).value);
			}
		}
	};

	struct add_country_modifier_complex_effect {
		effect_parsing_environment& env;

		int16_t duration = 0;
		modifiers::national_modifier_tag name;

		add_country_modifier_complex_effect(effect_parsing_environment& e) : env(e) {}

		void set_name(const token_and_type& t) {
			name = tag_from_text(
				env.s.modifiers_m.named_national_modifiers_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}

		void finalize() const {
			if(env.current_scope.main_slot == trigger_slot_contents::nation) {
				if(duration <= 0) {
					env.data.push_back(effect_codes::add_country_modifier_no_duration);
					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(name).value);
				} else {
					env.data.push_back(effect_codes::add_country_modifier);
					env.data.push_back(3ui16);
					env.data.push_back(trigger_payload(name).value);
					env.data.push_back(trigger_payload(duration).value);
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::province) {
				if(duration <= 0) {
					env.data.push_back(effect_codes::add_country_modifier_province_no_duration);
					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(name).value);
				} else {
					env.data.push_back(effect_codes::add_country_modifier_province);
					env.data.push_back(3ui16);
					env.data.push_back(trigger_payload(name).value);
					env.data.push_back(trigger_payload(duration).value);
				}
			} else {
				EFFECT_ERROR(invalid_scope_for_effect, env);
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
					env.s.culture_m.national_tags_index,
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

				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
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

				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(months).value);
			} else if(is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::casus_belli_tag);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::casus_belli_int);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(type).value);
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
					env.s.culture_m.national_tags_index,
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

				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
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

				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(months).value);
			} else if (is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::add_casus_belli_tag);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::add_casus_belli_int);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(months).value);
				env.data.push_back(trigger_payload(province_target).value);
			}
		}
	};
	struct remove_casus_belli_effect {
		effect_parsing_environment& env;

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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
					env.s.culture_m.national_tags_index,
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
				env.data.push_back(trigger_payload(type).value);
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
				env.data.push_back(trigger_payload(type).value);
			} else if (is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::remove_casus_belli_tag);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::remove_casus_belli_int);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(province_target).value);
			}
		}
	};
	struct this_remove_casus_belli_effect {
		effect_parsing_environment& env;

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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
					env.s.culture_m.national_tags_index,
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
				env.data.push_back(trigger_payload(type).value);
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
				env.data.push_back(trigger_payload(type).value);
			} else if (is_valid_index(target_tag)) {
				env.data.push_back(effect_codes::this_remove_casus_belli_tag);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
				env.data.push_back(trigger_payload(target_tag).value);
			} else {
				env.data.push_back(effect_codes::this_remove_casus_belli_int);
				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(type).value);
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
				env.s.culture_m.national_tags_index,
				cultures::tag_to_encoding(t.start, t.end));
		}
		void set_casus_belli(const token_and_type& t) {
			casus_belli = tag_from_text(
				env.s.military_m.named_cb_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
					env.s.culture_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end));
			}
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			bool use_target_tag = !from_target && !this_target;

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

			env.data.push_back(uint16_t(3 + (use_target_tag  ? 1 : 0) + 3 * attacker_goals.size() + 3 * defender_goals.size()));
			if(use_target_tag)
				env.data.push_back(trigger_payload(target_tag).value);
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
			if(env.current_scope.main_slot == trigger_slot_contents::nation) {
				if(days <= 0) {
					if(env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::country_event_immediate_this_nation);
					else if(env.current_scope.this_slot == trigger_slot_contents::state)
						env.data.push_back(effect_codes::country_event_immediate_this_state);
					else if(env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::country_event_immediate_this_province);
					else if(env.current_scope.this_slot == trigger_slot_contents::pop)
						env.data.push_back(effect_codes::country_event_immediate_this_pop);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(
						env.ecm.register_triggered_event(
							env.s.event_m,
							id,
							trigger_scope_state{ trigger_slot_contents::nation, trigger_slot_contents::nation, env.current_scope.this_slot })).value);
				} else {
					if(env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::country_event_this_nation);
					else if(env.current_scope.this_slot == trigger_slot_contents::state)
						env.data.push_back(effect_codes::country_event_this_state);
					else if(env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::country_event_this_province);
					else if(env.current_scope.this_slot == trigger_slot_contents::pop)
						env.data.push_back(effect_codes::country_event_this_pop);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(3ui16);
					env.data.push_back(trigger_payload(
						env.ecm.register_triggered_event(
							env.s.event_m,
							id,
							trigger_scope_state{ trigger_slot_contents::nation, trigger_slot_contents::nation, env.current_scope.this_slot })).value);
					env.data.push_back(uint16_t(days));
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::province) {
				if(days <= 0) {
					if(env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::country_event_immediate_province_this_nation);
					else if(env.current_scope.this_slot == trigger_slot_contents::state)
						env.data.push_back(effect_codes::country_event_immediate_province_this_state);
					else if(env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::country_event_immediate_province_this_province);
					else if(env.current_scope.this_slot == trigger_slot_contents::pop)
						env.data.push_back(effect_codes::country_event_immediate_province_this_pop);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(2ui16);
					env.data.push_back(trigger_payload(
						env.ecm.register_triggered_event(
							env.s.event_m,
							id,
							trigger_scope_state{ trigger_slot_contents::nation, trigger_slot_contents::nation, env.current_scope.this_slot })).value);
				} else {
					if(env.current_scope.this_slot == trigger_slot_contents::nation)
						env.data.push_back(effect_codes::country_event_province_this_nation);
					else if(env.current_scope.this_slot == trigger_slot_contents::state)
						env.data.push_back(effect_codes::country_event_province_this_state);
					else if(env.current_scope.this_slot == trigger_slot_contents::province)
						env.data.push_back(effect_codes::country_event_province_this_province);
					else if(env.current_scope.this_slot == trigger_slot_contents::pop)
						env.data.push_back(effect_codes::country_event_province_this_pop);
					else
						EFFECT_ERROR(invalid_scope_for_effect, env);

					env.data.push_back(3ui16);
					env.data.push_back(trigger_payload(
						env.ecm.register_triggered_event(
							env.s.event_m,
							id,
							trigger_scope_state{ trigger_slot_contents::nation, trigger_slot_contents::nation, env.current_scope.this_slot })).value);
					env.data.push_back(uint16_t(days));
				}
			} else {
				EFFECT_ERROR(invalid_scope_for_effect, env);
			}
		}
	};
	struct province_event_complex_effect {
		effect_parsing_environment& env;

		int16_t days = 0;
		int32_t id = 0;

		province_event_complex_effect(effect_parsing_environment& e) : env(e) {}

		void finalize() const {
			if(env.current_scope.main_slot != trigger_slot_contents::province)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			if (days <= 0) {
				if(env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::province_event_immediate_this_nation);
				else if(env.current_scope.this_slot == trigger_slot_contents::state)
					env.data.push_back(effect_codes::province_event_immediate_this_state);
				else if(env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::province_event_immediate_this_province);
				else if(env.current_scope.this_slot == trigger_slot_contents::pop)
					env.data.push_back(effect_codes::province_event_immediate_this_pop);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(2ui16);
				env.data.push_back(trigger_payload(
					env.ecm.register_triggered_event(
						env.s.event_m,
						id,
						trigger_scope_state{ trigger_slot_contents::province, trigger_slot_contents::province, env.current_scope.this_slot })).value);
			} else {
				if(env.current_scope.this_slot == trigger_slot_contents::nation)
					env.data.push_back(effect_codes::province_event_this_nation);
				else if(env.current_scope.this_slot == trigger_slot_contents::state)
					env.data.push_back(effect_codes::province_event_this_state);
				else if(env.current_scope.this_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::province_event_this_province);
				else if(env.current_scope.this_slot == trigger_slot_contents::pop)
					env.data.push_back(effect_codes::province_event_this_pop);
				else
					EFFECT_ERROR(invalid_scope_for_effect, env);

				env.data.push_back(3ui16);
				env.data.push_back(trigger_payload(
					env.ecm.register_triggered_event(
						env.s.event_m,
						id,
						trigger_scope_state{ trigger_slot_contents::province, trigger_slot_contents::province, env.current_scope.this_slot })).value);
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
		issues::option_tag value;

		dominant_issue_effect(effect_parsing_environment& e) : env(e) {}

		void set_value(const token_and_type& t) {
			value = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}

		void finalize() const {
			if(env.current_scope.main_slot == trigger_slot_contents::pop) {
				env.data.push_back(effect_codes::dominant_issue);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(value).value);
				add_float_to_payload(env.data, factor);
			} else if(env.current_scope.main_slot == trigger_slot_contents::nation) {
				env.data.push_back(effect_codes::dominant_issue_nation);
				env.data.push_back(4ui16);
				env.data.push_back(trigger_payload(value).value);
				add_float_to_payload(env.data, factor);
			} else {
				EFFECT_ERROR(invalid_scope_for_effect, env);
			}
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}

		void finalize() const {
			if (env.current_scope.main_slot != trigger_slot_contents::nation)
				EFFECT_ERROR(invalid_scope_for_effect, env);

			env.data.push_back(effect_codes::upper_house);
			env.data.push_back(3ui16);
			env.data.push_back(trigger_payload(ideology).value);
			env.data.push_back(trigger_payload(int16_t(value * 100.0f)).value);
		}
	};
	struct scaled_militancy_effect {
		effect_parsing_environment& env;

		float factor = 0.0f;
		float unemployment = 0.0f;
		ideologies::ideology_tag ideology;
		issues::option_tag issue;

		scaled_militancy_effect(effect_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_issue(const token_and_type& t) {
			issue = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}

		void finalize() const {
			if(env.current_scope.main_slot == trigger_slot_contents::pop) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_militancy_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_militancy_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_militancy_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::nation) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_militancy_nation_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_militancy_nation_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_militancy_nation_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::state) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_militancy_state_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_militancy_state_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_militancy_state_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::province) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_militancy_province_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_militancy_province_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_militancy_province_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else {
				EFFECT_ERROR(invalid_scope_for_effect, env);

			}
		}
	};
	struct scaled_consciousness_effect {
		effect_parsing_environment& env;

		float factor = 0.0f;
		float unemployment = 0.0f;
		ideologies::ideology_tag ideology;
		issues::option_tag issue;

		scaled_consciousness_effect(effect_parsing_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_issue(const token_and_type& t) {
			issue = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}

		void finalize() const {
			if(env.current_scope.main_slot == trigger_slot_contents::pop) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_consciousness_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_consciousness_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_consciousness_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::nation) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_consciousness_nation_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_consciousness_nation_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_consciousness_nation_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::state) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_consciousness_state_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_consciousness_state_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_consciousness_state_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else if(env.current_scope.main_slot == trigger_slot_contents::province) {
				if(is_valid_index(ideology)) {
					env.data.push_back(effect_codes::scaled_consciousness_province_ideology);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(ideology).value);
					add_float_to_payload(env.data, factor);
				} else if(is_valid_index(issue)) {
					env.data.push_back(effect_codes::scaled_consciousness_province_issue);
					env.data.push_back(4ui16);
					env.data.push_back(trigger_payload(issue).value);
					add_float_to_payload(env.data, factor);
				} else {
					env.data.push_back(effect_codes::scaled_consciousness_province_unemployment);
					env.data.push_back(3ui16);
					add_float_to_payload(env.data, factor * unemployment);
				}
			} else {
				EFFECT_ERROR(invalid_scope_for_effect, env);
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_background(const token_and_type& t) {
			background = tag_from_text(
				env.s.military_m.named_leader_trait_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_name(const token_and_type& t) {
			name = text_data::get_thread_safe_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_background(const token_and_type& t) {
			background = tag_from_text(
				env.s.military_m.named_leader_trait_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_name(const token_and_type& t) {
			name = text_data::get_thread_safe_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
		issues::option_tag from;
		issues::option_tag to;

		move_issue_percentage_effect(effect_parsing_environment& e) : env(e) {}

		void set_from(const token_and_type& t) {
			from = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_to(const token_and_type& t) {
			to = tag_from_text(
				env.s.issues_m.named_option_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
			
			env.data.push_back(5ui16);
			env.data.push_back(trigger_payload(from).value);
			env.data.push_back(trigger_payload(to).value);
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
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
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
				if (env.current_scope.main_slot == trigger_slot_contents::province /*&& env.current_scope.from_slot == trigger_slot_contents::nation*/)
					env.data.push_back(effect_codes::party_loyalty_province_from_nation);
				else if(/*env.current_scope.main_slot == trigger_slot_contents::nation &&*/ env.current_scope.from_slot == trigger_slot_contents::province)
					env.data.push_back(effect_codes::party_loyalty_nation_from_province);
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

#ifdef _DEBUG
	struct unknown_effect_scope {};
#endif


	inline trigger_tag read_limit_trigger(const token_group* s, const token_group* e, effect_parsing_environment& env) {
		const auto trigger_data = parse_trigger(env.s, env.current_scope, s, e);
		return commit_trigger(env.s.trigger_m, trigger_data);
	}
	inline modifiers::factor_tag read_ai_chance(const token_group* s, const token_group* e, effect_parsing_environment& env) {
		return modifiers::parse_modifier_factors(
			env.s,
			env.current_scope,
			1.0f,
			0.0f,
			s,
			e);
	}

	struct common_effect_scope_base {
		effect_parsing_environment& env;
		trigger_scope_state scope_state;
		size_t payload_size_offset = 0ui64;

		common_effect_scope_base(effect_parsing_environment& e) : env(e) {}

		void add_simple_effect_f(const std::tuple<token_and_type, association_type, token_and_type>& args) {
			add_simple_effect(
				env,
				std::get<0>(args),
				std::get<1>(args),
				std::get<2>(args));
		}
		void finalize() const {
			env.data[payload_size_offset] = uint16_t(env.data.size() - payload_size_offset);
		}
		void add_limit(trigger_tag limit) {
			env.data[payload_size_offset + 1] = trigger_payload(limit).value;
		}
		template<typename T>
		void add_scope(const T& other) {
			other.finalize();
			env.current_scope = scope_state;
		}
		template<typename T>
		void add_complex_effect(const T& other) {
			other.finalize();
		}
	};

	 struct variable_name_effect_scope_reading_object : public common_effect_scope_base {
		 variable_name_effect_scope_reading_object(const token_and_type& name, effect_parsing_environment& e) : common_effect_scope_base(e) {
			const auto left_handle = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, name.start, name.end);

			if (const auto region = tag_from_text(env.s.province_m.named_states_index, left_handle); is_valid_index(region)) {
				scope_state = trigger_scope_state{
					trigger_slot_contents::province,
					e.current_scope.this_slot,
					e.current_scope.from_slot,
					};
				e.current_scope = scope_state;
				env.data.push_back(uint16_t(effect_codes::region_scope | effect_codes::is_scope | effect_codes::scope_has_limit));
				env.data.push_back(3ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(trigger_payload(trigger_tag()).value);
				env.data.push_back(trigger_payload(region).value);
			} else if (const auto pop_type = tag_from_text(env.s.population_m.named_pop_type_index, left_handle); is_valid_index(pop_type)) {
				if(e.current_scope.main_slot == trigger_slot_contents::nation)
					env.data.push_back(uint16_t(effect_codes::pop_type_scope_nation | effect_codes::is_scope | effect_codes::scope_has_limit));
				else if (e.current_scope.main_slot == trigger_slot_contents::state)
					env.data.push_back(uint16_t(effect_codes::pop_type_scope_state | effect_codes::is_scope | effect_codes::scope_has_limit));
				else if (e.current_scope.main_slot == trigger_slot_contents::province)
					env.data.push_back(uint16_t(effect_codes::pop_type_scope_province | effect_codes::is_scope | effect_codes::scope_has_limit));
				else
					EFFECT_ERROR(invalid_scope_for_effect, e);

				scope_state = trigger_scope_state{
					trigger_slot_contents::pop,
					e.current_scope.this_slot,
					e.current_scope.from_slot,
					};
				e.current_scope = scope_state;

				env.data.push_back(3ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(trigger_payload(trigger_tag()).value);
				env.data.push_back(trigger_payload(pop_type).value);
			} else if (const auto tag = tag_from_text(e.s.culture_m.national_tags_index, cultures::tag_to_encoding(name.start, name.end)); is_valid_index(tag)) {
				scope_state = trigger_scope_state{
					trigger_slot_contents::nation,
					e.current_scope.this_slot,
					e.current_scope.from_slot,
					 };
				e.current_scope = scope_state;
				env.data.push_back(uint16_t(effect_codes::tag_scope | effect_codes::is_scope | effect_codes::scope_has_limit));
				env.data.push_back(3ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(trigger_payload(trigger_tag()).value);
				env.data.push_back(trigger_payload(tag).value);
			} else if (is_integer(name.start, name.end)) {
				scope_state = trigger_scope_state{
					trigger_slot_contents::province,
					e.current_scope.this_slot,
					e.current_scope.from_slot,
					 };
				e.current_scope = scope_state;
				env.data.push_back(uint16_t(effect_codes::integer_scope | effect_codes::is_scope | effect_codes::scope_has_limit));
				env.data.push_back(3ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(trigger_payload(trigger_tag()).value);
				env.data.push_back(token_to<uint16_t>(name));
			} else {
				EFFECT_ERROR(unknown_effect_scope, e);
			}
		}
	};

	template<typename scope_trigger>
	struct effect_scope_reading_object : public common_effect_scope_base {
		effect_scope_reading_object(effect_parsing_environment& e) : common_effect_scope_base(e) {
			const auto code = scope_trigger::produce_code(e.current_scope);
			if (code) {
				e.data.push_back(uint16_t(*code | effect_codes::is_scope | effect_codes::scope_has_limit));
				e.data.push_back(2ui16);
				payload_size_offset = e.data.size() - 1;
				env.data.push_back(trigger_payload(trigger_tag()).value);
			} else {
				EFFECT_ERROR(unknown_effect_scope, e);
			}

			scope_state = scope_trigger::produce_new_scope(e.current_scope);
			e.current_scope = scope_state;
		}
	};

	struct option_reader : public effect_scope_reading_object<generic_scope_effect> {
		text_data::text_tag name;
		modifiers::factor_tag ai_chance;

		option_reader(effect_parsing_environment& e) : effect_scope_reading_object<generic_scope_effect>(e) {}

		void set_name(const token_and_type& t) {
			name = text_data::get_thread_safe_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
		}
	};

	struct random_effect : public common_effect_scope_base {
		uint16_t chance = 0ui16;

		random_effect(effect_parsing_environment& e) : common_effect_scope_base(e) {
			e.data.push_back(uint16_t(effect_codes::random_scope | effect_codes::is_scope | effect_codes::scope_has_limit));
			e.data.push_back(3ui16);
			payload_size_offset = e.data.size() - 1;
			env.data.push_back(trigger_payload(trigger_tag()).value);
			env.data.push_back(0ui16);

			scope_state = e.current_scope;
		}
		void finalize() const {
			env.data[payload_size_offset] = uint16_t(env.data.size() - payload_size_offset);
			env.data[payload_size_offset + 2] = chance;
		}
	};

	struct random_list_effect : public common_effect_scope_base {
		size_t last_subscope_start;

		random_list_effect(effect_parsing_environment& e) : common_effect_scope_base(e) {
			e.data.push_back(uint16_t(effect_codes::random_list_scope | effect_codes::is_scope));
			e.data.push_back(3ui16);
			payload_size_offset = e.data.size() - 1;
			env.data.push_back(0ui16);

			scope_state = e.current_scope;

			env.data.push_back(0ui16);
			last_subscope_start = e.data.size() - 1;
		}
		void add_list(uint16_t chance) {
			env.data[last_subscope_start] = chance;
			env.data.push_back(0ui16);
			last_subscope_start = env.data.size() - 1;
		}
		void finalize() const {
			env.data.pop_back();
			env.data[payload_size_offset + 1] = uint16_t(get_random_list_effect_chances_sum(&env.data[payload_size_offset-1]));
			env.data[payload_size_offset] = uint16_t(env.data.size() - payload_size_offset);
		}
	};

	inline const variable_name_effect_scope_reading_object& get_effect_vscope(const token_and_type&, association_type, const variable_name_effect_scope_reading_object& t) {
		return t;
	};
	inline uint16_t get_list_effect_chance(const token_and_type& t, association_type, const effect_scope_reading_object<generic_scope_effect>& sub_effect) {
		sub_effect.finalize();
		return token_to<uint16_t>(t);
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
MEMBER_DEF(triggers::add_province_modifier_complex_effect, duration, "duration");
MEMBER_FDEF(triggers::add_country_modifier_complex_effect, set_name, "name");
MEMBER_DEF(triggers::add_country_modifier_complex_effect, duration, "duration");
MEMBER_FDEF(triggers::casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::casus_belli_effect, set_target, "target");
MEMBER_DEF(triggers::casus_belli_effect, months, "months");
MEMBER_FDEF(triggers::add_casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::add_casus_belli_effect, set_target, "target");
MEMBER_DEF(triggers::add_casus_belli_effect, months, "months");
MEMBER_FDEF(triggers::remove_casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::remove_casus_belli_effect, set_target, "target");
MEMBER_FDEF(triggers::this_remove_casus_belli_effect, set_type, "type");
MEMBER_FDEF(triggers::this_remove_casus_belli_effect, set_target, "target");
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
MEMBER_DEF(triggers::random_effect, chance, "chance");
MEMBER_FDEF(triggers::random_list_effect, add_list, "add_list");
MEMBER_FDEF(triggers::option_reader, set_name, "name");
MEMBER_DEF(triggers::option_reader, ai_chance, "ai_chance");

template<typename T>
struct _set_member<CT_STRING("_add_effect"), T> {
	template<typename V>
	static void set(T& class_passed, V&& v) {
		class_passed.add_simple_effect_f(std::forward<V>(v));
	}
};
template<typename T>
struct _set_member<CT_STRING("_add_complex_effect"), T> {
	template<typename V>
	static void set(T& class_passed, V&& v) {
		class_passed.add_complex_effect(std::forward<V>(v));
	}
};
template<typename T>
struct _set_member<CT_STRING("_add_scope"), T> {
	template<typename V>
	static void set(T& class_passed, V&& v) {
		class_passed.add_scope(std::forward<V>(v));
	}
};
template<typename T>
struct _set_member<CT_STRING("_add_limit"), T> {
	static void set(T& class_passed, triggers::trigger_tag limit) {
		class_passed.add_limit(limit);
	}
};

#define EPAIR(x) typepair< CT_STRING( #x ), x ## _effect>

namespace triggers {
	using unsorted_effect_map = type_list <
		EPAIR(capital),
		EPAIR(add_core),
		EPAIR(remove_core),
		EPAIR(change_region_name),
		EPAIR(trade_goods),
		EPAIR(add_accepted_culture),
		EPAIR(primary_culture),
		EPAIR(remove_accepted_culture),
		EPAIR(life_rating),
		EPAIR(religion),
		EPAIR(is_slave),
		EPAIR(research_points),
		EPAIR(tech_school),
		EPAIR(government),
		EPAIR(treasury),
		EPAIR(war_exhaustion),
		EPAIR(prestige),
		EPAIR(change_tag),
		EPAIR(change_tag_no_core_switch),
		EPAIR(add_province_modifier),
		EPAIR(add_country_modifier),
		EPAIR(set_country_flag),
		EPAIR(clr_country_flag),
		EPAIR(country_event),
		EPAIR(province_event),
		EPAIR(military_access),
		EPAIR(badboy),
		EPAIR(secede_province),
		EPAIR(inherit),
		EPAIR(annex_to),
		EPAIR(release),
		EPAIR(change_controller),
		EPAIR(infrastructure),
		EPAIR(money),
		EPAIR(prestige_factor),
		EPAIR(leadership),
		EPAIR(create_vassal),
		EPAIR(end_military_access),
		EPAIR(leave_alliance),
		EPAIR(end_war),
		EPAIR(enable_ideology),
		EPAIR(ruling_party_ideology),
		EPAIR(plurality),
		EPAIR(remove_province_modifier),
		EPAIR(remove_country_modifier),
		EPAIR(create_alliance),
		EPAIR(release_vassal),
		EPAIR(change_province_name),
		EPAIR(enable_canal),
		EPAIR(set_global_flag),
		EPAIR(clr_global_flag),
		EPAIR(nationalvalue),
		EPAIR(civilized),
		EPAIR(election),
		EPAIR(social_reform),
		EPAIR(political_reform),
		EPAIR(add_tax_relative_income),
		EPAIR(neutrality),
		EPAIR(reduce_pop),
		EPAIR(move_pop),
		EPAIR(pop_type),
		EPAIR(years_of_research),
		EPAIR(military_reform),
		EPAIR(economic_reform),
		EPAIR(remove_random_military_reforms),
		EPAIR(remove_random_economic_reforms),
		EPAIR(add_crime),
		EPAIR(nationalize),
		EPAIR(build_factory_in_capital_state),
		EPAIR(activate_technology),
		EPAIR(great_wars_enabled),
		EPAIR(world_wars_enabled),
		EPAIR(assimilate),
		EPAIR(literacy),
		EPAIR(add_crisis_interest),
		EPAIR(flashpoint_tension),
		EPAIR(add_crisis_temperature),
		EPAIR(consciousness),
		EPAIR(militancy),
		EPAIR(rgo_size),
		EPAIR(fort),
		EPAIR(naval_base),
		typepair<CT_STRING("railroad"), infrastructure_effect>
	>;

	using effect_map = typename sorted<unsorted_effect_map>::type;
#ifdef _DEBUG
	struct mismatched_effect_payload_size {};
	struct no_effect_payload_value {};
	struct unknown_effect {};
#endif

	void add_simple_effect(
		effect_parsing_environment& env,
		const token_and_type& effect_name,
		association_type a,
		const token_and_type& effect_value) {

		if (!map_call_functions<effect_map>::template bt_scan_ci<bool>(effect_name.start, effect_name.end,
			[a, &effect_value, &env](auto type_arg) {
			const auto code = decltype(type_arg)::type::produce_code(env.current_scope, a, effect_value);
			if (code) {
				const auto data_size = effect_code_data_sizes[*code & effect_codes::code_mask];
				if (data_size == 0) {
					env.data.push_back(uint16_t(*code | effect_codes::no_payload));
				} else {
					env.data.push_back(*code);
					env.data.push_back(static_cast<uint16_t>(data_size + 1));

					const auto payload = decltype(type_arg)::type::read_value(effect_value, env.s, env.current_scope, env.ecm);

					if (std::holds_alternative<int32_t>(payload)) {
						add_int32_t_to_payload(env.data, std::get<int32_t>(payload));
#ifdef _DEBUG
						if (data_size != 2)
							throw mismatched_effect_payload_size();
#endif
					} else if (std::holds_alternative<float>(payload)) {
						add_float_to_payload(env.data, std::get<float>(payload));
#ifdef _DEBUG
						if (data_size != 2)
							throw mismatched_effect_payload_size();
#endif
					} else if (std::holds_alternative<trigger_payload>(payload)) {
						env.data.push_back(std::get<trigger_payload>(payload).value);
#ifdef _DEBUG
						if (data_size != 1)
							throw mismatched_effect_payload_size();
#endif
					} else {
#ifdef _DEBUG
						throw no_effect_payload_value();
#endif
					}
				}
			} else {
				EFFECT_ERROR(invalid_scope_for_effect, env);
			}
			return true;
		})) {
			//not found in map
			if (is_fixed_token_ci(effect_name, "set_province_flag") || is_fixed_token_ci(effect_name, "clr_province_flag")) {
				//discard
			} else {
				const auto left_handle = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, effect_name.start, effect_name.end);

				if (const auto tech = tag_from_text(env.s.technology_m.named_technology_index, left_handle); is_valid_index(tech)) {
					if (env.current_scope.main_slot == trigger_slot_contents::nation) {
						env.data.push_back(effect_codes::variable_tech_name);
						env.data.push_back(2ui16);
						env.data.push_back(trigger_payload(tech).value);
					} else {
						EFFECT_ERROR(invalid_scope_for_effect, env);
					}
				} else if (const auto good = tag_from_text(env.s.economy_m.named_goods_index, left_handle); is_valid_index(good)) {
					if (env.current_scope.main_slot == trigger_slot_contents::nation) {
						env.data.push_back(effect_codes::variable_good_name);
						env.data.push_back(4ui16);
						env.data.push_back(trigger_payload(good).value);
						add_float_to_payload(env.data, token_to<float>(effect_value));
					} else if(env.current_scope.main_slot == trigger_slot_contents::province) {
						env.data.push_back(effect_codes::variable_good_name_province);
						env.data.push_back(4ui16);
						env.data.push_back(trigger_payload(good).value);
						add_float_to_payload(env.data, token_to<float>(effect_value));
					} else {
						EFFECT_ERROR(invalid_scope_for_effect, env);
					}
				} else {
					EFFECT_ERROR(unknown_effect, env);
				}
			}
		}
	}

	BEGIN_DOMAIN(effect_reading)
		BEGIN_TYPE(common_effect_scope_base)
		    MEMBER_VARIABLE_ASSOCIATION("_add_effect", accept_all, yield_full)
		    MEMBER_TYPE_EXTERN("_add_limit", "limit", trigger_tag, read_limit_trigger)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "trigger_revolt", trigger_revolt_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "diplomatic_influence", diplomatic_influence_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "relation", relation_effect)
		    MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "modify_relation", relation_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "add_province_modifier", add_province_modifier_complex_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "add_country_modifier", add_country_modifier_complex_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "casus_belli", casus_belli_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "add_casus_belli", add_casus_belli_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "remove_casus_belli", remove_casus_belli_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "this_remove_casus_belli", this_remove_casus_belli_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "war", war_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "country_event", country_event_complex_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "province_event", province_event_complex_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "sub_unit", sub_unit_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "set_variable", set_variable_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "change_variable", change_variable_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "ideology", ideology_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "dominant_issue", dominant_issue_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "upper_house", upper_house_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "scaled_militancy", scaled_militancy_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "scaled_consciousness", scaled_consciousness_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "define_general", define_general_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "define_admiral", define_admiral_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "add_war_goal", add_war_goal_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "move_issue_percentage", move_issue_percentage_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "party_loyalty", party_loyalty_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "build_railway_in_capital", build_railway_in_capital_effect)
			MEMBER_TYPE_ASSOCIATION("_add_complex_effect", "build_fort_in_capital", build_fort_in_capital_effect)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "hidden_tooltip", effect_scope_reading_object<generic_scope_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_neighbor_province", effect_scope_reading_object<any_neighbor_province_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_neighbor_country", effect_scope_reading_object<any_neighbor_country_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_country", effect_scope_reading_object<any_country_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_country", effect_scope_reading_object<random_country_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_neighbor_province", effect_scope_reading_object<random_neighbor_province_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_empty_neighbor_province", effect_scope_reading_object<random_empty_neighbor_province_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_greater_power", effect_scope_reading_object<any_greater_power_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "poor_strata", effect_scope_reading_object<poor_strata_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "middle_strata", effect_scope_reading_object<middle_strata_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "rich_strata", effect_scope_reading_object<rich_strata_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_pop", effect_scope_reading_object<random_pop_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_owned", effect_scope_reading_object<random_owned_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_province", effect_scope_reading_object<random_owned_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_owned", effect_scope_reading_object<any_owned_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "all_core", effect_scope_reading_object<all_core_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_state", effect_scope_reading_object<any_state_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_state", effect_scope_reading_object<random_state_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "any_pop", effect_scope_reading_object<any_pop_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "owner", effect_scope_reading_object<owner_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "controller", effect_scope_reading_object<controller_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "location", effect_scope_reading_object<location_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "country", effect_scope_reading_object<country_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "capital_scope", effect_scope_reading_object<capital_scope_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "this", effect_scope_reading_object<this_scope_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "from", effect_scope_reading_object<from_scope_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "sea_zone", effect_scope_reading_object<sea_zone_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "cultural_union", effect_scope_reading_object<cultural_union_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "overlord", effect_scope_reading_object<overlord_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "sphere_owner", effect_scope_reading_object<sphere_owner_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "independence", effect_scope_reading_object<independence_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "flashpoint_tag_scope", effect_scope_reading_object<flashpoint_tag_scope_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "crisis_state_scope", effect_scope_reading_object<crisis_state_scope_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "state_scope", effect_scope_reading_object<state_scope_effect>)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random", random_effect)
			MEMBER_TYPE_ASSOCIATION("_add_scope", "random_list", random_list_effect)
			MEMBER_VARIABLE_TYPE_ASSOCIATION("_add_scope", accept_all, variable_name_effect_scope_reading_object, get_effect_vscope)
		END_TYPE
		BEGIN_TYPE(random_list_effect)
		    MEMBER_VARIABLE_TYPE_ASSOCIATION("add_list", accept_all, effect_scope_reading_object<generic_scope_effect>, get_list_effect_chance)
		END_TYPE
		BEGIN_TYPE(random_effect)
		    INHERIT_FROM(common_effect_scope_base)
		    MEMBER_ASSOCIATION("chance", "chance", value_from_rh<uint16_t>)
		END_TYPE
		BEGIN_TYPE(variable_name_effect_scope_reading_object)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<generic_scope_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<any_neighbor_province_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<any_neighbor_country_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<any_country_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<random_country_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<random_neighbor_province_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<random_empty_neighbor_province_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<any_greater_power_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<poor_strata_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<middle_strata_effect>)
			INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<rich_strata_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<random_pop_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<random_owned_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<any_owned_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<all_core_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<any_state_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<random_state_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<any_pop_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<owner_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<controller_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<location_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<country_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<capital_scope_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<this_scope_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<from_scope_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<sea_zone_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<cultural_union_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<overlord_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<sphere_owner_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<independence_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<flashpoint_tag_scope_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<crisis_state_scope_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(effect_scope_reading_object<state_scope_effect>)
		    INHERIT_FROM(common_effect_scope_base)
		END_TYPE
		BEGIN_TYPE(option_reader)
			INHERIT_FROM(common_effect_scope_base)
			MEMBER_ASSOCIATION("name", "name", token_from_rh)
			MEMBER_TYPE_EXTERN("ai_chance", "ai_chance", modifiers::factor_tag, read_ai_chance)
		END_TYPE
		BEGIN_TYPE(trigger_revolt_effect)
			MEMBER_ASSOCIATION("culture", "culture", token_from_rh)
			MEMBER_ASSOCIATION("religion", "religion", token_from_rh)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
		END_TYPE
		BEGIN_TYPE(diplomatic_influence_effect)
			MEMBER_ASSOCIATION("who", "who", token_from_rh)
			MEMBER_ASSOCIATION("value", "value", value_from_rh<int16_t>)
		END_TYPE
		BEGIN_TYPE(relation_effect)
			MEMBER_ASSOCIATION("who", "who", token_from_rh)
			MEMBER_ASSOCIATION("who", "tag", token_from_rh)
			MEMBER_ASSOCIATION("who", "with", token_from_rh)
			MEMBER_ASSOCIATION("value", "value", value_from_rh<int16_t>)
			MEMBER_ASSOCIATION("value", "relation", value_from_rh<int16_t>)
		END_TYPE
		BEGIN_TYPE(add_province_modifier_complex_effect)
			MEMBER_ASSOCIATION("name", "name", token_from_rh)
			MEMBER_ASSOCIATION("duration", "duration", value_from_rh<int16_t>)
		END_TYPE
		BEGIN_TYPE(add_country_modifier_complex_effect)
			MEMBER_ASSOCIATION("name", "name", token_from_rh)
			MEMBER_ASSOCIATION("duration", "duration", value_from_rh<int16_t>)
		END_TYPE
		BEGIN_TYPE(casus_belli_effect)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("target", "target", token_from_rh)
			MEMBER_ASSOCIATION("months", "months", value_from_rh<int16_t>)
		END_TYPE
		BEGIN_TYPE(add_casus_belli_effect)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("target", "target", token_from_rh)
			MEMBER_ASSOCIATION("months", "months", value_from_rh<int16_t>)
		END_TYPE
		BEGIN_TYPE(remove_casus_belli_effect)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("target", "target", token_from_rh)
		END_TYPE
		BEGIN_TYPE(this_remove_casus_belli_effect)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("target", "target", token_from_rh)
		END_TYPE
		BEGIN_TYPE(limited_wg_reader)
			MEMBER_ASSOCIATION("country", "country", token_from_rh)
			MEMBER_ASSOCIATION("casus_belli", "casus_belli", token_from_rh)
			MEMBER_ASSOCIATION("state_province_id", "state_province_id", value_from_rh<uint16_t>)
		END_TYPE
		BEGIN_TYPE(war_effect)
			MEMBER_ASSOCIATION("target", "target", token_from_rh)
			MEMBER_TYPE_ASSOCIATION("defender_goal", "defender_goal", limited_wg_reader)
			MEMBER_TYPE_ASSOCIATION("attacker_goal", "attacker_goal", limited_wg_reader)
			MEMBER_ASSOCIATION("call_ally", "call_ally", value_from_rh<bool>)
		END_TYPE
		BEGIN_TYPE(country_event_complex_effect)
			MEMBER_ASSOCIATION("days", "days", value_from_rh<int16_t>)
			MEMBER_ASSOCIATION("id", "id", value_from_rh<int32_t>)
		END_TYPE
		BEGIN_TYPE(province_event_complex_effect)
			MEMBER_ASSOCIATION("days", "days", value_from_rh<int16_t>)
			MEMBER_ASSOCIATION("id", "id", value_from_rh<int32_t>)
		END_TYPE
		BEGIN_TYPE(sub_unit_effect)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("value", "value", token_from_rh)
		END_TYPE
		BEGIN_TYPE(set_variable_effect)
			MEMBER_ASSOCIATION("which", "which", token_from_rh)
			MEMBER_ASSOCIATION("value", "value", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(change_variable_effect)
			MEMBER_ASSOCIATION("which", "which", token_from_rh)
			MEMBER_ASSOCIATION("value", "value", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(ideology_effect)
			MEMBER_ASSOCIATION("value", "value", token_from_rh)
			MEMBER_ASSOCIATION("factor", "factor", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(dominant_issue_effect)
			MEMBER_ASSOCIATION("value", "value", token_from_rh)
			MEMBER_ASSOCIATION("factor", "factor", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(upper_house_effect)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("value", "value", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(scaled_militancy_effect)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("issue", "issue", token_from_rh)
			MEMBER_ASSOCIATION("factor", "factor", value_from_rh<float>)
			MEMBER_ASSOCIATION("factor", "value", value_from_rh<float>)
			MEMBER_ASSOCIATION("unemployment", "unemployment", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(scaled_consciousness_effect)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("issue", "issue", token_from_rh)
			MEMBER_ASSOCIATION("factor", "factor", value_from_rh<float>)
			MEMBER_ASSOCIATION("factor", "value", value_from_rh<float>)
			MEMBER_ASSOCIATION("unemployment", "unemployment", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(define_general_effect)
			MEMBER_ASSOCIATION("name", "name", token_from_rh)
			MEMBER_ASSOCIATION("background", "background", token_from_rh)
			MEMBER_ASSOCIATION("personality", "personality", token_from_rh)
		END_TYPE
		BEGIN_TYPE(define_admiral_effect)
			MEMBER_ASSOCIATION("name", "name", token_from_rh)
			MEMBER_ASSOCIATION("background", "background", token_from_rh)
			MEMBER_ASSOCIATION("personality", "personality", token_from_rh)
		END_TYPE
		BEGIN_TYPE(add_war_goal_effect)
			MEMBER_ASSOCIATION("casus_belli", "casus_belli", token_from_rh)
		END_TYPE
		BEGIN_TYPE(move_issue_percentage_effect)
			MEMBER_ASSOCIATION("from", "from", token_from_rh)
			MEMBER_ASSOCIATION("to", "to", token_from_rh)
			MEMBER_ASSOCIATION("value", "value", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(party_loyalty_effect)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("province_id", "province_id", value_from_rh<uint16_t>)
			MEMBER_ASSOCIATION("loyalty_value", "loyalty_value", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(build_railway_in_capital_effect)
			MEMBER_ASSOCIATION("in_whole_capital_state", "in_whole_capital_state", value_from_rh<bool>)
			MEMBER_ASSOCIATION("limit_to_world_greatest_level", "limit_to_world_greatest_level", value_from_rh<bool>)
		END_TYPE
		BEGIN_TYPE(build_fort_in_capital_effect)
			MEMBER_ASSOCIATION("in_whole_capital_state", "in_whole_capital_state", value_from_rh<bool>)
			MEMBER_ASSOCIATION("limit_to_world_greatest_level", "limit_to_world_greatest_level", value_from_rh<bool>)
		END_TYPE
	END_DOMAIN;

	bool effect_scope_has_single_member(const uint16_t* source) { //precondition: scope known to not be empty
		const auto data_offset = 2 + effect_scope_data_payload(source[0]);
		return get_effect_payload_size(source) == data_offset + get_effect_payload_size(source + data_offset);
	}

	int32_t simplify_effect(uint16_t* source) {
		if ((source[0] & effect_codes::is_scope) != 0) {
			auto source_size = 1 + get_effect_payload_size(source);

			if ((source[0] & effect_codes::code_mask) == effect_codes::random_list_scope) {
				auto sub_units_start = source + 4; // [code] + [payload size] + [chances total] + [first sub effect chance]

				while (sub_units_start < source + source_size) {
					const auto old_size = 1 + get_effect_payload_size(sub_units_start);
					const auto new_size = simplify_effect(sub_units_start);

					if (new_size != old_size) { // has been simplified, assumes that new size always <= old size
						std::copy(sub_units_start + old_size, source + source_size, sub_units_start + new_size);
						source_size -= (old_size - new_size);
					}
					sub_units_start += new_size + 1;
				}
			} else {
				auto sub_units_start = source + 2 + effect_scope_data_payload(source[0]);

				while (sub_units_start < source + source_size) {
					const auto old_size = 1 + get_effect_payload_size(sub_units_start);
					const auto new_size = simplify_effect(sub_units_start);

					if (new_size != old_size) { // has been simplified, assumes that new size always <= old size
						std::copy(sub_units_start + old_size, source + source_size, sub_units_start + new_size);
						source_size -= (old_size - new_size);
					}
					sub_units_start += new_size;
				}
			}

			source[1] = uint16_t(source_size - 1);

			if ((source[0] & effect_codes::scope_has_limit) != 0 && !is_valid_index(trigger_payload(source[2]).trigger)) {
				std::copy(source + 3, source + source_size, source + 2);
				--source_size;
				source[0] = uint16_t(source[0] & ~effect_codes::scope_has_limit);
				source[1] = uint16_t(source_size - 1);
			}

			if ((source[0] & effect_codes::code_mask) == effect_codes::generic_scope) {
				if (source_size == 2) {
					return 0; //simplify empty scope to nothing
				} else if (((source[0] & effect_codes::scope_has_limit) == 0) && effect_scope_has_single_member(source)) {
					std::copy(source + 2, source + source_size, source);
					source_size -= 2;
				}
			}

			return source_size;
		} else {
			return 1 + get_effect_payload_size(source); // non scopes cannot be simplified
		}
	}

	std::vector<uint16_t> parse_effect(
		scenario::scenario_manager& s,
		events::event_creation_manager& ecm,
		trigger_scope_state outer_scope,
		const token_group* start,
		const token_group* end) {

		effect_parsing_environment parse_env(s, ecm, outer_scope);

		auto effect = parse_object<effect_scope_reading_object<generic_scope_effect>, effect_reading>(start, end, parse_env);
		effect.finalize();
		parse_env.data.push_back(0ui16);

		const auto new_size = simplify_effect(parse_env.data.data());
		parse_env.data.resize(static_cast<size_t>(new_size));
		return std::move(parse_env.data);
	}

	effect_tag commit_effect(trigger_manager& trigger_manager, const std::vector<uint16_t>& new_effect) {
		if (new_effect.size() == 0ui64)
			return effect_tag();

		const auto search_result = std::search(trigger_manager.effect_data.begin(), trigger_manager.effect_data.end(), new_effect.begin(), new_effect.end());
		if (search_result != trigger_manager.effect_data.end()) {
			return effect_tag(static_cast<value_base_of<trigger_tag>>(search_result - trigger_manager.effect_data.begin()));
		} else {
			trigger_manager.effect_data.pop_back();
			const auto new_start_pos = trigger_manager.effect_data.size();
			trigger_manager.effect_data.insert(trigger_manager.effect_data.end(), new_effect.begin(), new_effect.end());
			trigger_manager.effect_data.push_back(0ui16);
			return effect_tag(static_cast<value_base_of<trigger_tag>>(new_start_pos));
		}
	}

	raw_event_option parse_option_effect(
		scenario::scenario_manager& s,
		events::event_creation_manager& ecm,
		trigger_scope_state outer_scope,
		const token_group* start,
		const token_group* end) {

		effect_parsing_environment parse_env(s, ecm, outer_scope);

		auto effect = parse_object<option_reader, effect_reading>(start, end, parse_env);
		effect.finalize();
		parse_env.data.push_back(0ui16);

		const auto new_size = simplify_effect(parse_env.data.data());
		parse_env.data.resize(static_cast<size_t>(new_size));

		raw_event_option result;

		result.effect = commit_effect(s.trigger_m, parse_env.data);
		result.ai_chance = effect.ai_chance;
		result.name = effect.name;

		return result;
	}
}
