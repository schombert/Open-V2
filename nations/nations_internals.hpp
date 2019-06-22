#pragma once
#include "common\common.h"
#include "nations\nations.h"

namespace nations {
	template<typename world_state_t>
	RELEASE_INLINE int32_t internal_get_relationship(world_state_t const& ws, country_tag a, country_tag b) {
		if(auto f = ws.find(ws.get<nation::relations>(a), relationship{ b, 0i16 }); f)
			return f->value;
		return 0;
	}
	template<typename world_state_t>
	RELEASE_INLINE float internal_get_influence_value(world_state_t const& ws, country_tag a, country_tag b) {
		if(auto f = ws.find(ws.get<nation::gp_influence>(a), influence(b)); f)
			return f->amount;
		return 0.0f;
	}
	template<typename world_state_t>
	RELEASE_INLINE int32_t internal_get_influence_level(world_state_t const& ws, country_tag a, country_tag b) {
		if(auto f = ws.find(ws.get<nation::gp_influence>(a), influence(b)); f)
			return f->level();
		return 2;
	}
	template<typename world_state_t>
	RELEASE_INLINE void internal_set_relationship(world_state_t& ws, country_tag a, country_tag b, int32_t value) {
		auto& a_relations = ws.get<nation::relations>(a);
		if(auto f = ws.find( a_relations, relationship{ b, 0i16 }); f)
			f->value = int16_t(value);
		else
			ws.add_item(a_relations, relationship{ b, int16_t(value) });

		auto& b_relations = ws.get<nation::relations>(b);
		if(auto f = ws.find(b_relations, relationship{ a, 0i16 }); f)
			f->value = int16_t(value);
		else
			ws.add_item(b_relations, relationship{ a, int16_t(value) });
	}
	template<typename world_state_t>
	RELEASE_INLINE void internal_adjust_relationship(world_state_t& ws, country_tag a, country_tag b, int32_t value) {
		auto& a_relations = ws.get<nation::relations>(a);
		if(auto f = ws.find(a_relations, relationship{ b, 0i16 }); f)
			f->value = int16_t(std::clamp(value + f->value, -200, 200));
		else
			ws.add_item(a_relations, relationship{ b, int16_t(std::clamp(value, -200, 200)) });

		auto& b_relations = ws.get<nation::relations>(b);
		if(auto f = ws.find(b_relations, relationship{ a, 0i16 }); f)
			f->value = int16_t(std::clamp(value + f->value, -200, 200));
		else
			ws.add_item(b_relations, relationship{ a, int16_t(std::clamp(value, -200, 200)) });
	}
	template<typename world_state_t>
	RELEASE_INLINE void internal_set_influence(world_state_t& ws, country_tag a, country_tag b, float value, int32_t level) {
		auto& a_inf = ws.get<nation::gp_influence>(a);
		auto f = ws.find(a_inf, influence(b));
		if(f == nullptr) {
			ws.add_item(a_inf, influence{ 0.0f, b, 0.0f, int8_t(2), 0i8 });
			ws.add_item(ws.get<nation::influencers>(b), a);

			f = ws.find(a_inf, influence(b));
		}

		f->amount = value;

		if(f->level() == 5 && level != 5) {
			ws.set<nation::sphere_leader>(b, country_tag());
			ws.remove_item(ws.get<nation::sphere_members>(a), b);
			f->level(level);
		} else if(f->level() != 5 && level == 5) {
			auto& b_leader = ws.get<nation::sphere_leader>(b);
			if(b_leader == country_tag()) {
				b_leader = a;
				ws.add_item(ws.get<nation::sphere_members>(a), b);
				f->level(5);
			} else {
				f->level(4);
			}
		} else {
			f->level(level);
		}
	}
	template<typename world_state_t>
	RELEASE_INLINE void internal_set_influence_value(world_state_t& ws, country_tag a, country_tag b, float value) {
		auto& a_inf = ws.get<nation::gp_influence>(a);
		if(auto f = ws.find(a_inf, influence(b)); f) {
			f->amount = value;
		} else {
			ws.add_item(a_inf, influence{ 0.0f, b, value, int8_t(2), 0i8 });
			ws.add_item(ws.get<nation::influencers>(b), a);
		}
	}
}
