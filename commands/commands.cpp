#include "common\\common.h"
#include "commands.hpp"
#include "world_state\\world_state.h"

namespace commands {
	set_budget::set_budget(nations::country_tag n, set_budget_type t, int8_t v) : nation_for(n), type(t) {
		switch(t) {
			default:
			case set_budget_type::all:
				rich_tax = v;
				middle_tax = v;
				poor_tax = v;
				social_spending = v;
				administrative_spending = v;
				education_spending = v;
				military_spending = v;
				tarrifs = v;
				debt_setting = v;
				army_stockpile_spending = v;
				navy_stockpile_spending = v;
				projects_stockpile_spending = v;
				return;
			case set_budget_type::rich_tax:
				rich_tax = v;
				return;
			case set_budget_type::middle_tax:
				middle_tax = v;
				return;
			case set_budget_type::poor_tax:
				poor_tax = v;
				return;
			case set_budget_type::social_spending:
				social_spending = v;
				return;
			case set_budget_type::administrative_spending:
				administrative_spending = v;
				return;
			case set_budget_type::education_spending:
				education_spending = v;
				return;
			case set_budget_type::military_spending:
				military_spending = v;
				return;
			case set_budget_type::tarrifs:
				tarrifs = v;
				return;
			case set_budget_type::debt_setting:
				debt_setting = v;
				return;
			case set_budget_type::army_stockpile_spending:
				army_stockpile_spending = v;
				return;
			case set_budget_type::navy_stockpile_spending:
				navy_stockpile_spending = v;
				return;
			case set_budget_type::projects_stockpile_spending:
				projects_stockpile_spending = v;
				return;
		}
	}

	set_budget::set_budget(nations::country_tag n, int8_t rich_tax_v, int8_t middle_tax_v, int8_t poor_tax_v,
		int8_t social_spending_v, int8_t administrative_spending_v, int8_t education_spending_v,
		int8_t military_spending_v, int8_t tarrifs_v, int8_t debt_setting_v, int8_t army_stockpile_spending_v,
		int8_t navy_stockpile_spending_v, int8_t projects_stockpile_spending_v)  : nation_for(n),
		rich_tax(rich_tax_v), middle_tax(middle_tax_v), poor_tax(poor_tax_v), social_spending(social_spending_v),
		administrative_spending(administrative_spending_v), education_spending(education_spending_v),
		military_spending(military_spending_v), tarrifs(tarrifs_v), debt_setting(debt_setting_v),
		army_stockpile_spending(army_stockpile_spending_v), navy_stockpile_spending(navy_stockpile_spending_v),
		projects_stockpile_spending(projects_stockpile_spending_v)
	{}

	void execute_command(set_budget const& c, world_state& ws) {
		if(ws.w.nation_s.nations.is_valid_index(c.nation_for)) {
			auto& n = ws.w.nation_s.nations[c.nation_for];

			switch(c.type) {
				default:
				case set_budget_type::all:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.rich_tax = std::clamp(c.rich_tax, int8_t(min_v), int8_t(max_v));
					n.middle_tax = std::clamp(c.middle_tax, int8_t(min_v), int8_t(max_v));
					n.poor_tax = std::clamp(c.poor_tax, int8_t(min_v), int8_t(max_v));
				}
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_social_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_social_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_social_spending])
						: 100, min_v, 100);
					n.social_spending = std::clamp(c.social_spending, int8_t(min_v), int8_t(max_v));
				}
					n.administrative_spending = std::clamp(c.administrative_spending, int8_t(0), int8_t(100));
					n.education_spending = std::clamp(c.education_spending, int8_t(0), int8_t(100));
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_military_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_military_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_military_spending])
						: 100, min_v, 100);
					n.military_spending = std::clamp(c.military_spending, int8_t(min_v), int8_t(max_v));
				}
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tariff]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tariff] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tariff])
						: 100, min_v, 100);
					n.tarrifs = std::clamp(c.tarrifs, int8_t(min_v), int8_t(max_v));
				}
					n.debt_setting = std::clamp(c.debt_setting, int8_t(-100), int8_t(100));
					n.army_stockpile_spending = std::clamp(c.army_stockpile_spending, int8_t(0), int8_t(100));
					n.navy_stockpile_spending = std::clamp(c.navy_stockpile_spending, int8_t(0), int8_t(100));
					n.projects_stockpile_spending = std::clamp(c.projects_stockpile_spending, int8_t(0), int8_t(100));
					return;

				case set_budget_type::rich_tax:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.rich_tax = std::clamp(c.rich_tax, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::middle_tax:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.middle_tax = std::clamp(c.middle_tax, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::poor_tax:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.poor_tax = std::clamp(c.poor_tax, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::social_spending:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_social_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_social_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_social_spending])
						: 100, min_v, 100);
					n.social_spending = std::clamp(c.social_spending, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::administrative_spending:
					n.administrative_spending = std::clamp(c.administrative_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::education_spending:
					n.education_spending = std::clamp(c.education_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::military_spending:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_military_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_military_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_military_spending])
						: 100, min_v, 100);
					n.military_spending = std::clamp(c.military_spending, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::tarrifs:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tariff]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tariff] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tariff])
						: 100, min_v, 100);
					n.tarrifs = std::clamp(c.tarrifs, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::debt_setting:
					n.debt_setting = std::clamp(c.debt_setting, int8_t(-100), int8_t(100));
					return;
				case set_budget_type::army_stockpile_spending:
					n.army_stockpile_spending = std::clamp(c.army_stockpile_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::navy_stockpile_spending:
					n.navy_stockpile_spending = std::clamp(c.navy_stockpile_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::projects_stockpile_spending:
					n.projects_stockpile_spending = std::clamp(c.projects_stockpile_spending, int8_t(0), int8_t(100));
					return;
			}
		}
	}
}
