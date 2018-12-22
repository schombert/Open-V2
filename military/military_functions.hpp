#pragma once
#include "military_functions.h"

namespace military {
	template<typename T, typename U>
	auto has_military_access_with(world_state const& ws, T nation_by, U nation_target) -> decltype(ve::widen_to<T, U>(true)) {
		return
			(ve::load(nation_by, ws.w.nation_s.nations.get_row<nation::overlord>()) == nation_target)
			| (ve::load(nation_target, ws.w.nation_s.nations.get_row<nation::overlord>()) == nation_by)
			| ve::apply(nation_by, nation_target, [&ws](nations::country_tag b, nations::country_tag t) {
				return in_war_with(ws, b, t);
		});
	}


	template<typename T>
	auto province_is_contested(world_state const& ws, T this_province) -> decltype(ve::widen_to<T>(true)) {
		return ve::load(this_province, ws.w.province_s.province_state_container.get_row<province_state::siege_progress>()) != 0.0f;
	}
}