#include "triggers.h"
#include "scenario\\scenario.h"
#include <optional>
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"

namespace triggers {

	//for reading simple association, non scope triggers
	namespace codes {
		// non scopes
		constexpr uint16_t year = 0x0001;
		constexpr uint16_t month = 0x0002;
		constexpr uint16_t port = 0x0003;
		constexpr uint16_t rank = 0x0004;
		constexpr uint16_t invention = 0x0005;
		constexpr uint16_t strata = 0x0006;
		constexpr uint16_t life_rating_province = 0x0007;
		constexpr uint16_t life_rating_state = 0x0008;
		//scopes
	}

	namespace pop_strata {
		constexpr int32_t poor = 0;
		constexpr int32_t middle = 1;
		constexpr int32_t rich = 2;
	}

	struct year_trigger {
		static constexpr bool bool_type = false;
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			return codes::year;
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{token_to<int32_t>(t)};
		}
	};

	struct month_trigger {
		static constexpr bool bool_type = false;
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			return codes::month;
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};

	struct port_trigger {
		static constexpr bool bool_type = true;
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return codes::port;
			else
				return std::optional<uint16_t>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};

	struct rank_trigger {
		static constexpr bool bool_type = false;
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return codes::rank;
			else
				return std::optional<uint16_t>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct invention_trigger {
		static constexpr bool bool_type = false;
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return codes::invention;
			else
				return std::optional<uint16_t>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.technology_m.named_invention_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)) );
		}
	};

	struct strata_trigger {
		static constexpr bool bool_type = false;
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return codes::strata;
			else
				return std::optional<uint16_t>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			if (is_fixed_token_ci(t.start, t.end, "poor")) {
				return trigger_bytecode(pop_strata::poor);
			} else if (is_fixed_token_ci(t.start, t.end, "rich")) {
				return trigger_bytecode(pop_strata::rich);
			} else {
				return trigger_bytecode(pop_strata::middle);
			}
		}
	};
	struct life_raing_trigger {
		static constexpr bool bool_type = false;
		static std::optional<uint16_t> produce_code(const trigger_scope_state& scope) {
			if (scope.main_slot == trigger_slot_contents::province)
				return codes::life_rating_province;
			else if (scope.main_slot == trigger_slot_contents::state)
				return codes::life_rating_state;
			else
				return std::optional<uint16_t>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};

	//for scope triggers
	struct or_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope) {

		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {

		}
	};
}
