#include "common\\common.h"
#include "technologies_functions.hpp"
#include "nations\\nations.h"
#include "world_state\\world_state.h"
#include "modifiers\\modifier_functions.h"
#include "military\\military_functions.h"
#include "concurrency_tools\\concurrency_tools.h"
#include "concurrency_tools\\ve.h"
#include "world_state\\messages.h"
#include "nations\\nations_functions.hpp"
#include <random>

namespace technologies {
	void init_technology_state(world_state& ws) {
		ws.w.technology_s.discovery_count.resize(ws.s.technology_m.technologies_container.size());
		std::fill_n(ws.w.technology_s.discovery_count.data(), ws.s.technology_m.technologies_container.size(), 0);
	}

	void apply_tech_modifiers(world_state& ws, nations::country_tag this_nation, tech_definition const& def) {
		for(uint32_t i = 0; i < tech_definition_size; ++i)
			ws.w.nation_s.tech_attributes.get(this_nation, def.offsets[i]) += def.values[i];
	}

	void apply_technology(world_state& ws, nations::country_tag this_nation, tech_tag tech) {
		technology& t = ws.s.technology_m.technologies_container[tech];
		auto nation_id = this_nation;

		ws.w.technology_s.discovery_count[tech] += 1;

		ws.w.nation_s.active_technologies.set(nation_id, tech, true);

		apply_tech_modifiers(ws, this_nation, t.attributes);

		if(is_valid_index(t.production_adjustment)) {
			ve::accumulate(ws.s.economy_m.goods_count,
				ws.w.nation_s.production_adjustments.get_row(nation_id),
				ws.s.technology_m.production_adjustments.get_row(t.production_adjustment), ve::serial_exact());
		}
		if(is_valid_index(t.rebel_adjustment)) {
			ve::accumulate(uint32_t(ws.s.population_m.rebel_types.size()),
				ws.w.nation_s.rebel_org_gain.get_row(nation_id),
				ws.s.technology_m.rebel_org_gain.get_row(t.rebel_adjustment), ve::serial_exact());
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
			bit_vector_set(ws.w.nation_s.active_goods.get_row(nation_id), good, true);
		}
		if(is_valid_index(t.enable_crime)) {
			for(int32_t i = int32_t(ws.s.modifiers_m.crimes.size()); i--; ) {
				if(ws.s.modifiers_m.crimes[uint32_t(i)].modifier == t.enable_crime) {
					bit_vector_set((bitfield_type*)(&(ws.w.nation_s.nations.get<nation::enabled_crimes>(this_nation))), uint32_t(i), true);
					break;
				}
			}
		}
		if((t.flags & technology::gas_defence) != 0)
			ws.w.nation_s.nations.set<nation::has_gas_defence>(this_nation, true);
		if((t.flags & technology::gas_attack) != 0)
			ws.w.nation_s.nations.set<nation::has_gas_attack>(this_nation, true);
	}


	void apply_single_technology(world_state& ws, nations::country_tag this_nation, tech_tag tech) {
		if(ws.w.nation_s.active_technologies.get(this_nation, tech) == true)
			return;

		apply_technology(ws, this_nation, tech);
		if(is_valid_index(ws.s.technology_m.technologies_container[tech].unit_adjustment))
			military::update_all_unit_attributes(ws, this_nation);
		ws.w.nation_s.nations.get<nation::base_prestige>(this_nation) += ws.s.technology_m.technologies_container[tech].shared_prestige / float(ws.w.technology_s.discovery_count[tech]);
		
		// to be caught by standard update
		//if(is_valid_index(ws.s.technology_m.technologies_container[tech].modifier))
		//	modifiers::reset_national_modifier(ws, this_nation);
	}

	void reset_technologies(world_state& ws, nations::country_tag this_nation) {
		for(uint32_t i = 0; i < tech_offset::count; ++i)
			ws.w.nation_s.tech_attributes.get(this_nation, i) = 0.0f;

		auto tech_count = int32_t(ws.s.technology_m.technologies_container.size());
		for(int32_t i = 0; i < tech_count; ++i) {
			ws.w.nation_s.active_technologies.set(this_nation, technologies::tech_tag(technologies::tech_tag::value_base_t(i)), false);
		}
		//auto tech_row = ws.w.nation_s.active_technologies.get_row(this_nation);
		//Eigen::Map<Eigen::Matrix<uint64_t, -1, 1>>(tech_row.data(), ws.w.nation_s.active_technologies.inner_size) =
		//	Eigen::Matrix<uint64_t, -1, 1>::Zero(ws.w.nation_s.active_technologies.inner_size);

		auto active_goods = ws.w.nation_s.active_goods.get_row(this_nation);
		for(uint32_t j = 0; j < ws.s.economy_m.goods_count; ++j) {
			bit_vector_set(active_goods, economy::goods_tag(economy::goods_tag::value_base_t(j)), 0 == (ws.s.economy_m.goods[economy::goods_tag(static_cast<economy::goods_tag::value_base_t>(j))].flags & economy::good_definition::not_available_from_start));
		}

		bitfield_type* enabled_crimes = (bitfield_type*)(&(ws.w.nation_s.nations.get<nation::enabled_crimes>(this_nation)));
		for(int32_t i = int32_t(ws.s.modifiers_m.crimes.size()); i--; ) {
			if(ws.s.modifiers_m.crimes[uint32_t(i)].default_active)
				bit_vector_set(enabled_crimes, uint32_t(i), true);
			else
				bit_vector_set(enabled_crimes, uint32_t(i), false);
		}

		const auto production_adjustment_count = ws.s.economy_m.goods_count * uint32_t(production_adjustment::production_adjustment_count);
		Eigen::Map<Eigen::Matrix<float, -1, 1>>(
			ws.w.nation_s.production_adjustments.get_row(this_nation).data(),
			production_adjustment_count) =
			Eigen::Matrix<float, -1, 1>::Zero(production_adjustment_count);

		const auto rebel_types_count = int32_t(ws.s.population_m.rebel_types.size());
		Eigen::Map<Eigen::Matrix<float, -1, 1>>(
			ws.w.nation_s.rebel_org_gain.get_row(this_nation).data(),
			rebel_types_count) = Eigen::Matrix<float, -1, 1>::Zero(rebel_types_count);
	}

	void restore_technologies(world_state& ws, nations::country_tag this_nation) {
		//auto tech_row = ws.w.nation_s.active_technologies.get_row(this_nation);
		for(int32_t i = int32_t(ws.s.technology_m.technologies_container.size()); i--; ) {
			auto tag = tech_tag(tech_tag::value_base_t(i));
			if(ws.w.nation_s.active_technologies.get(this_nation, tag)) {
				apply_technology(ws, this_nation, tag);
			}
		}

		military::update_all_unit_attributes(ws, this_nation);
	}

	

	float effective_tech_cost(tech_tag t, world_state const& ws, nations::country_tag this_nation) {
		auto& tech = ws.s.technology_m.technologies_container[t];
		auto base_cost = float(tech.cost);
		auto years_after_unlock_adjustment = std::max(0.0f, 1.0f - float(int32_t(tag_to_date(ws.w.current_date).year()) - int32_t(tech.year)) / ws.s.modifiers_m.global_defines.tech_year_span);
		
		float modifier = [&ws, &tech, this_nation](){
			if(tech.category == tech_category_type::army)
				return 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::army_tech_research_bonus>(this_nation);
			if(tech.category == tech_category_type::navy)
				return 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::navy_tech_research_bonus>(this_nation);
			if(tech.category == tech_category_type::commerce)
				return 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::commerce_tech_research_bonus>(this_nation);
			if(tech.category == tech_category_type::industry)
				return 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::industry_tech_research_bonus>(this_nation);
			if(tech.category == tech_category_type::culture)
				return 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::culture_tech_research_bonus>(this_nation);
			else
				return 1.0f;
		}();
		float vassal_factor = [&ws, t, ol = ws.w.nation_s.nations.get<nation::overlord>(this_nation)]() {
			if(ws.w.nation_s.nations.is_valid_index(ol)) {
				return ws.w.nation_s.active_technologies.get(ol, t) ? ws.s.modifiers_m.global_defines.tech_factor_vassal : 1.0f;
			}
			return 1.0f;
		}();
		return years_after_unlock_adjustment * vassal_factor* base_cost / modifier;
	}

	float get_invention_chance(tech_tag t, world_state& ws, nations::country_tag this_nation) {
		return modifiers::test_additive_factor(ws.s.technology_m.technologies_container[t].ai_chance, ws, this_nation, triggers::const_parameter()) / 100.0f;
	}
	ve::fp_vector get_invention_chance(tech_tag t, world_state& ws, ve::contiguous_tags_base<nations::country_tag> this_nation) {
		return modifiers::test_contiguous_additive_factor(ws.s.technology_m.technologies_container[t].ai_chance, ws, this_nation, ve::contiguous_tags_base<union_tag>());
	}

	bool can_research(tech_tag t, world_state const& ws, nations::country_tag this_nation) {
		auto id = this_nation;
		auto id_valid = ws.w.nation_s.nations.is_valid_index(id);
		if(id_valid && ws.w.nation_s.active_technologies.get(id, t))
			return false;
		auto& tech = ws.s.technology_m.technologies_container[t];
		if(tech.year != 0 && tag_to_date(ws.w.current_date).year() < tech.year)
			return false;
		if(is_valid_index(tech.allow) && !triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(tech.allow),
			ws, this_nation, this_nation, triggers::const_parameter()))
			return false;
		if(is_valid_index(tech.preceeding) && id_valid && !ws.w.nation_s.active_technologies.get(id, tech.preceeding))
			return false;

		return true;
	}

	void daily_update(world_state& ws) {
		ve::execute_serial<nations::country_tag>(ws.w.nation_s.nations.vector_size(), [
			&ws,
			progress_row = ws.w.nation_s.nations.get_row<nation::research_points>()
		](auto off) {
				const auto old_value = ve::load(off, progress_row);
				ve::store(off, progress_row, daily_research_points(ws, off, old_value));
			});

		concurrency::concurrent_queue<std::pair<nations::country_tag, technologies::tech_tag >> pending_new_techs;

		ws.w.nation_s.nations.parallel_for_each([&ws, &pending_new_techs](nations::country_tag n) {
			if(auto t = ws.w.nation_s.nations.get<nation::current_research>(n); t) {
				auto tech_cost = effective_tech_cost(t, ws, n);
				auto rp = ws.w.nation_s.nations.get<nation::research_points>(n);

				if(rp >= tech_cost) {
					// todo: decide new ai research
					ws.w.nation_s.nations.set<nation::current_research>(n, technologies::tech_tag());
					ws.w.nation_s.nations.set<nation::research_points>(n, rp - tech_cost);
					pending_new_techs.push(std::pair<nations::country_tag, technologies::tech_tag >(n, t));

					messages::new_technology(ws, n, t);
				}
			}
		});

		auto const invention_offset = to_index(ws.w.current_date) & 31;
		concurrency::parallel_for(uint32_t(invention_offset), uint32_t(ws.s.technology_m.inventions.size()), 32ui32, [&ws, &pending_new_techs](uint32_t index) {
			auto const this_invention = ws.s.technology_m.inventions[index];
			auto const vsize = ws.w.nation_s.nations.vector_size();
			ve::execute_serial_fast<nations::country_tag>(vsize, [&ws, &pending_new_techs, vsize, this_invention](auto tags) {
				auto const inventions_researched = ve::mask_vector(ve::load(tags, ws.w.nation_s.active_technologies.get_row(this_invention, vsize)))
					& !(nations::nation_exists(ws, tags)); // if nation does not exist -> mark as invented to ignore

				if(ve::compress_mask(inventions_researched) != ve::full_mask) {
					auto const invention_allowed = triggers::test_contiguous_trigger(
						ws.s.trigger_m.trigger_data.data() + to_index(ws.s.technology_m.technologies_container[this_invention].allow),
						ws,
						tags, tags, ve::contiguous_tags<union_tag>()) & !inventions_researched;
					if(ve::compress_mask(invention_allowed) != ve::empty_mask) {
						auto const invention_chance = ve::select(invention_allowed, get_invention_chance(this_invention, ws, tags), 0.0f);
						ve::apply(tags, invention_chance, [&pending_new_techs, &ws, this_invention](nations::country_tag n, float chance) {
							if(chance > 0.0f) {
								std::uniform_real_distribution<float> dist(0, 1.0f);
								auto const random_result = dist(get_local_generator());
								if(random_result <= chance) {
									pending_new_techs.push(std::pair<nations::country_tag, technologies::tech_tag >(n, this_invention));

									messages::new_invention(ws, n, this_invention);
								}
							}
						});
					}
				}
			});
			auto const nations = ve::contiguous_tags_base<nations::country_tag>(uint32_t(index));
			
		});

		std::pair<nations::country_tag, technologies::tech_tag> p;
		while(pending_new_techs.try_pop(p)) {
			technologies::apply_single_technology(ws, p.first, p.second);
		}
	}
}
