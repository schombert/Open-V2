#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "gui\\gui.h"

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
		int8_t army_stockpile_spending = 0i8;
		int8_t navy_stockpile_spending = 0i8;
		int8_t projects_stockpile_spending = 0i8;

		set_budget(nations::country_tag n, set_budget_type t, int8_t v);
		set_budget(nations::country_tag n, int8_t rich_tax_v, int8_t middle_tax_v, int8_t poor_tax_v,
			int8_t social_spending_v, int8_t administrative_spending_v, int8_t education_spending_v,
			int8_t military_spending_v, int8_t tarrifs_v, int8_t army_stockpile_spending_v,
			int8_t navy_stockpile_spending_v, int8_t projects_stockpile_spending_v);
	};

	enum class province_building_type : uint8_t {
		province_railroad,
		province_fort,
		province_naval_base,
		state_railroad,
		state_fort
	};

	struct province_building {
		nations::country_tag nation_for;
		provinces::province_tag p;
		nations::state_tag s;

		province_building_type type = province_building_type::province_railroad;
		province_building(nations::country_tag n, province_building_type t, provinces::province_tag prov) : nation_for(n), type(t), p(prov) {}
		province_building(nations::country_tag n, province_building_type t, nations::state_tag st) : nation_for(n), type(t), s(st) {}
	};


	void execute_command(set_budget const& c, world_state& ws);
	void execute_command(province_building const& c, world_state& ws);

	bool is_command_valid(province_building const& c, world_state const& ws);
	ui::xy_pair explain_command_conditions(province_building const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);
	
	using full_command_set = command_set<set_budget, province_building>;
}
