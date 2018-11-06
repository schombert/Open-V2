#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"

class world_state;

namespace commands {
	template<typename ... Ts>
	class command_set;

	template<>
	class command_set<> {
	public:
		bool execute(world_state&) { return false; }
	};

	template<typename FIRST, typename ... REST>
	class command_set<FIRST, REST ...> : public command_set <REST...> {
	public:
		fixed_sz_list<FIRST, 1024, 16> command_list;

		template<typename T, typename ... ARGS>
		void add(ARGS&& ... args);
		bool execute(world_state& ws);
	};
	
	enum class set_budget_type : uint8_t {
		all,
		rich_tax,
		middle_tax,
		poor_tax,
		social_spending,
		administrative_spending,
		education_spending,
		military_spending,
		tarrifs,
		debt_setting,
		army_stockpile_spending,
		navy_stockpile_spending,
		projects_stockpile_spending
	};

	struct set_budget {
		nations::country_tag nation_for;
		set_budget_type type = set_budget_type::all;
		int8_t rich_tax = 0i8;
		int8_t middle_tax = 0i8;
		int8_t poor_tax = 0i8;
		int8_t social_spending = 0i8;
		int8_t administrative_spending = 0i8;
		int8_t education_spending = 0i8;
		int8_t military_spending = 0i8;
		int8_t tarrifs = 0i8;
		int8_t debt_setting = 0i8;
		int8_t army_stockpile_spending = 0i8;
		int8_t navy_stockpile_spending = 0i8;
		int8_t projects_stockpile_spending = 0i8;

		set_budget(nations::country_tag n, set_budget_type t, int8_t v);
		set_budget(nations::country_tag n, int8_t rich_tax_v, int8_t middle_tax_v, int8_t poor_tax_v,
			int8_t social_spending_v, int8_t administrative_spending_v, int8_t education_spending_v,
			int8_t military_spending_v, int8_t tarrifs_v, int8_t debt_setting_v, int8_t army_stockpile_spending_v,
			int8_t navy_stockpile_spending_v, int8_t projects_stockpile_spending_v);
	};

	void execute_command(set_budget const& c, world_state& ws);

	using full_command_set = command_set<set_budget>;
}
