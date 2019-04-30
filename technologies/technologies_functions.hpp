#pragma once
#include "common\\common.h"
#include "technologies_functions.h"
#include "concurrency_tools\\ve.h"
#include "world_state\\world_state.h"

namespace technologies {
	template<typename T, typename U>
	auto daily_research_points(world_state const& ws, T this_nation, U base) -> decltype(ve::widen_to<T>(0.0f)) {
		auto const total_pop = ve::load(this_nation, ws.w.nation_s.nations.get_row<nation::total_core_population>());
		auto const inv_total_pop = ve::select(total_pop > 0.0f, 1.0f / total_pop, 0.0f);

		decltype(ve::widen_to<T>(0.0f)) points_by_type = 0.0f;

		for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
			population::pop_type_tag this_tag(static_cast<population::pop_type_tag::value_base_t>(i));
			auto& pt = ws.s.population_m.pop_types[this_tag];
			auto const r_points = pt.research_points;
			auto const r_optimum = pt.research_optimum;

			if(r_points != 0 && r_optimum != 0.0f) {
				const auto pop_by_type = ve::apply(this_nation, [&ws, this_tag](nations::country_tag n) {
					if(ws.w.nation_s.nations.is_valid_index(n)) {
						return  ws.w.nation_s.nation_demographics.get(n, population::to_demo_tag(ws, this_tag));
					} else {
						return 0.0f;
					}
				});
				points_by_type = ve::multiply_and_add(r_points, ve::min(1.0f, pop_by_type * inv_total_pop * (1.0f / r_optimum)), points_by_type);
			}
		}

		auto const rp_modifier = ve::load(this_nation, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::research_points>(0));
		auto const rp_modifier_modifier = ve::load(this_nation, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::research_points_modifier>(0));
		return ve::multiply_and_add(rp_modifier + points_by_type,  rp_modifier_modifier + 1.0f, base);
	}

}
