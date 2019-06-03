#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "gui\\gui.h"
#include "triggers\triggers.h"

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

		province_building(nations::country_tag n, province_building_type t, provinces::province_tag prov) : nation_for(n), p(prov), type(t) {}
		province_building(nations::country_tag n, province_building_type t, nations::state_tag st) : nation_for(n), s(st), type(t) {}
	};

	struct foreign_invest_railroad {
		nations::country_tag nation_by;
		provinces::province_tag p;

		foreign_invest_railroad(nations::country_tag b, provinces::province_tag prov) : nation_by(b), p(prov) {}
	};

	struct fabricate_cb {
		nations::country_tag nation_for;
		nations::country_tag nation_target;
		military::cb_type_tag type;

		fabricate_cb(nations::country_tag f, nations::country_tag t, military::cb_type_tag y) : nation_for(f), nation_target(t), type(y) {}
	};

	struct change_research {
		nations::country_tag nation_for;
		technologies::tech_tag tech;

		change_research(nations::country_tag f, technologies::tech_tag t) : nation_for(f), tech(t) {}
	};

	struct change_influence_priority_level {
		nations::country_tag nation_for;
		nations::country_tag nation_target;
		int8_t level;

		change_influence_priority_level(nations::country_tag f, nations::country_tag t, int8_t l) : nation_for(f), nation_target(t), level(l) {}
	};

	struct execute_event {
		jsf_prng generator;
		triggers::const_parameter target;
		triggers::const_parameter from;
		events::event_tag e;
		int8_t option;

		execute_event(jsf_prng g, triggers::const_parameter t, triggers::const_parameter f, events::event_tag ev, int8_t o) : generator(g), target(t), from(f), e(ev), option(o) {}
	};

	struct change_sphere_leader {
		nations::country_tag nation_for;
		nations::country_tag new_leader;

		change_sphere_leader(nations::country_tag f, nations::country_tag l) : nation_for(f), new_leader(l) {}
	};

	struct change_ruling_party {
		nations::country_tag nation_for;
		governments::party_tag new_party;

		change_ruling_party(nations::country_tag f, governments::party_tag a) : nation_for(f), new_party(a) {}
	};

	struct set_reform {
		nations::country_tag nation_for;
		issues::option_tag reform;

		set_reform(nations::country_tag f, issues::option_tag r) : nation_for(f), reform(r) {}
	};

	struct build_factory {
		nations::country_tag by_nation;
		nations::state_tag in_state;
		economy::factory_type_tag factory_type;

		build_factory(nations::country_tag b, nations::state_tag s, economy::factory_type_tag f) : 
			by_nation(b), in_state(s), factory_type(f) {}
	};

	struct change_national_focus {
		nations::country_tag by_nation;
		nations::state_tag in_state;
		modifiers::national_focus_tag new_focus;

		change_national_focus(nations::country_tag b, nations::state_tag s, modifiers::national_focus_tag f) :
			by_nation(b), in_state(s), new_focus(f) {}
	};

	void execute_command(set_budget const& c, world_state& ws);
	void execute_command(province_building const& c, world_state& ws);
	void execute_command(fabricate_cb const& c, world_state& ws);
	void execute_command(change_research const& c, world_state& ws);
	void execute_command(execute_event const& c, world_state& ws);
	void execute_command(change_influence_priority_level const& c, world_state& ws);
	void execute_command(change_sphere_leader const& c, world_state& ws);
	void execute_command(change_ruling_party const& c, world_state& ws);
	void execute_command(set_reform const& c, world_state& ws);
	void execute_command(build_factory const& c, world_state& ws);
	void execute_command(change_national_focus const& c, world_state& ws);
	void execute_command(foreign_invest_railroad const& c, world_state& ws);

	bool is_command_valid(province_building const& c, world_state const& ws);
	bool is_command_valid(fabricate_cb const& c, world_state const& ws);
	bool is_command_valid(change_research const& c, world_state const& ws);
	bool is_command_valid(change_ruling_party const& c, world_state const& ws);
	bool is_command_valid(set_reform const& c, world_state const& ws);
	bool is_command_valid(build_factory const& c, world_state const& ws);
	bool is_command_valid(change_national_focus const& c, world_state const& ws);
	bool is_command_valid(foreign_invest_railroad const& c, world_state const& ws);

	ui::xy_pair explain_command_conditions(province_building const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);
	ui::xy_pair explain_command_conditions(fabricate_cb const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);
	ui::xy_pair explain_command_conditions(change_research const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);
	ui::xy_pair explain_command_conditions(set_reform const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);
	ui::xy_pair explain_command_conditions(build_factory const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);

	using full_command_set = command_set<set_budget, province_building, fabricate_cb, change_research, execute_event,
		change_influence_priority_level, change_sphere_leader, change_ruling_party, set_reform, build_factory, change_national_focus,
		foreign_invest_railroad>;
}
