#include "common\\common.h"
#include "technologies_functions.h"
#include "nations\\nations.h"
#include "world_state\\world_state.h"
#include "modifiers\\modifier_functions.h"
#include "military\\military_functions.h"

namespace technologies {
	void apply_technology(world_state& ws, nations::nation& this_nation, tech_tag tech) {
		technology& t = ws.s.technology_m.technologies_container[tech];
		auto nation_id = this_nation.id;

		bit_vector_set(ws.w.nation_s.active_technologies.get_row(nation_id), to_index(tech), true);

		this_nation.tech_attributes += t.attributes;

		if(is_valid_index(t.production_adjustment)) {
			const auto production_adjustment_count = ws.s.economy_m.goods_count * uint32_t(production_adjustment::production_adjustment_count);
			Eigen::Map<Eigen::Matrix<float, -1, 1>>(
				ws.w.nation_s.production_adjustments.get_row(nation_id),
				production_adjustment_count) +=
				Eigen::Map<Eigen::Matrix<float, -1, 1>>(
					ws.s.technology_m.production_adjustments.get_row(t.production_adjustment), 
					production_adjustment_count);
		}
		if(is_valid_index(t.rebel_adjustment)) {
			const auto rebel_types_count = int32_t(ws.s.population_m.rebel_types.size());
			Eigen::Map<Eigen::Matrix<float, -1, 1>>(
				ws.w.nation_s.rebel_org_gain.get_row(nation_id),
				rebel_types_count) +=
				Eigen::Map<Eigen::Matrix<float, -1, 1>>(
					ws.s.technology_m.rebel_org_gain.get_row(t.rebel_adjustment),
					rebel_types_count);
		}
		if(is_valid_index(t.unit_adjustment)) {
			for(uint32_t i = to_index(military::naval_unit_base) + 1ui32; i < ws.s.military_m.unit_types_count; ++i) {
				military::unit_type_tag this_unit(static_cast<military::unit_type_tag::value_base_t>(i));
				auto& stats_vector = ws.w.nation_s.unit_stats.get(nation_id, this_unit);
				stats_vector +=
					ws.s.technology_m.unit_type_adjustments.get(t.unit_adjustment, this_unit);
				auto unit_class = ws.s.military_m.unit_types[this_unit].flags & military::unit_type::class_mask;
				if((unit_class == military::unit_type::class_big_ship) |
					(unit_class == military::unit_type::class_light_ship) |
					(unit_class == military::unit_type::class_transport)) {
					//naval
					stats_vector += ws.s.technology_m.unit_type_adjustments.get(t.unit_adjustment, military::naval_unit_base);
				} else {
					//land
					stats_vector += ws.s.technology_m.unit_type_adjustments.get(t.unit_adjustment, military::army_unit_base);
				}
			}
		}

		if(is_valid_index(t.activate_factory)) {
			auto good = ws.s.economy_m.factory_types[t.activate_factory].output_good;
			bit_vector_set(ws.w.nation_s.active_goods.get_row(nation_id), to_index(good), true);
		}
		if(is_valid_index(t.enable_crime)) {
			for(int32_t i = int32_t(ws.s.modifiers_m.crimes.size()); i--; ) {
				if(ws.s.modifiers_m.crimes[uint32_t(i)].modifier == t.enable_crime) {
					bit_vector_set(&(this_nation.enabled_crimes), uint32_t(i), true);
					break;
				}
			}
		}
		if((t.flags & technology::gas_defence) != 0)
			this_nation.flags |= nations::nation::has_gas_defence;
		if((t.flags & technology::gas_attack) != 0)
			this_nation.flags |= nations::nation::has_gas_attack;
	}


	void apply_single_technology(world_state& ws, nations::nation& this_nation, tech_tag tech) {
		apply_technology(ws, this_nation, tech);
		if(is_valid_index(ws.s.technology_m.technologies_container[tech].unit_adjustment))
			military::update_all_unit_attributes(ws, this_nation);
		// to be caught by standard update
		//if(is_valid_index(ws.s.technology_m.technologies_container[tech].modifier))
		//	modifiers::reset_national_modifier(ws, this_nation);
	}

	void reset_technologies(world_state& ws, nations::nation& this_nation) {
		this_nation.tech_attributes = tech_attribute_vector::Zero();

		auto tech_row = ws.w.nation_s.active_technologies.get_row(this_nation.id);
		Eigen::Map<Eigen::Matrix<uint64_t, -1, 1>>(tech_row, ws.w.nation_s.active_technologies.inner_size) =
			Eigen::Matrix<uint64_t, -1, 1>::Zero(ws.w.nation_s.active_technologies.inner_size);

		auto active_goods = ws.w.nation_s.active_goods.get_row(this_nation.id);
		Eigen::Map<Eigen::Matrix<uint64_t, -1, 1>>(active_goods, ws.w.nation_s.active_goods.inner_size) =
			Eigen::Matrix<uint64_t, -1, 1>::Zero(ws.w.nation_s.active_goods.inner_size);

		for(int32_t i = int32_t(ws.s.modifiers_m.crimes.size()); i--; ) {
			if(ws.s.modifiers_m.crimes[uint32_t(i)].default_active)
				bit_vector_set(&(this_nation.enabled_crimes), uint32_t(i), true);
			else
				bit_vector_set(&(this_nation.enabled_crimes), uint32_t(i), false);
		}

		const auto production_adjustment_count = ws.s.economy_m.goods_count * uint32_t(production_adjustment::production_adjustment_count);
		Eigen::Map<Eigen::Matrix<float, -1, 1>>(
			ws.w.nation_s.production_adjustments.get_row(this_nation.id),
			production_adjustment_count) =
			Eigen::Matrix<float, -1, 1>::Zero(production_adjustment_count);

		const auto rebel_types_count = int32_t(ws.s.population_m.rebel_types.size());
		Eigen::Map<Eigen::Matrix<float, -1, 1>>(
			ws.w.nation_s.rebel_org_gain.get_row(this_nation.id),
			rebel_types_count) = Eigen::Matrix<float, -1, 1>::Zero(rebel_types_count);
	}

	void restore_technologies(world_state& ws, nations::nation& this_nation) {
		auto tech_row = ws.w.nation_s.active_technologies.get_row(this_nation.id);
		for(int32_t i = int32_t(ws.s.technology_m.technologies_container.size()); i--; ) {
			if(bit_vector_test(tech_row, uint32_t(i))) {
				apply_technology(ws, this_nation, tech_tag(static_cast<tech_tag::value_base_t>(i)));
			}
		}

		military::update_all_unit_attributes(ws, this_nation);
	}

	float daily_research_points(world_state const& ws, nations::nation const& this_nation) {
		// todo
		return 0.0f;
	}
}
