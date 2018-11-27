#include "common\\common.h"
#include "modifier_functions.h"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.h"

namespace modifiers {
	void add_unique_static_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod) {
		auto& smod = ws.w.nation_s.nations.get<nation::static_modifiers>(this_nation);
		if(contains_item(ws.w.nation_s.static_modifier_arrays, smod, mod) == false)
			add_item(ws.w.nation_s.static_modifier_arrays, smod, mod);
	}
	void add_static_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod) {
		add_item(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(this_nation), mod);
	}
	void add_static_modifier_to_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod) {
		auto& container = ws.w.province_s.province_state_container;
		add_item(ws.w.province_s.static_modifier_arrays, container.get<province_state::static_modifiers>(this_province), mod);
		
		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		auto owner = container.get<province_state::owner>(this_province);
		if(is_valid_index(nat_mod) && is_valid_index(owner)) {
			add_static_modifier_to_nation(ws, owner, nat_mod);
		}
	}

	void set_default_crimes(world_state& ws, nations::country_tag this_nation) {
		auto& enabled_crimes = ws.w.nation_s.nations.get<nation::enabled_crimes>(this_nation);
		enabled_crimes = 0ui64;
		for(int32_t i = int32_t(ws.s.modifiers_m.crimes.size()); i--; ) {
			if(ws.s.modifiers_m.crimes[uint32_t(i)].default_active)
				bit_vector_set(&(enabled_crimes), uint32_t(i), true);
		}
	}

	void apply_nat_modifier(world_state& ws, nations::country_tag this_nation, const modifier_definition& def) {
		for(uint32_t i = 0; i < modifier_definition_size; ++i)
			ws.w.nation_s.modifier_values.get(this_nation, def.offsets[i]) += def.values[i];
	}
	void apply_prov_modifier(world_state& ws, provinces::province_tag this_prov, const modifier_definition& def) {
		for(uint32_t i = 0; i < modifier_definition_size; ++i)
			ws.w.province_s.modifier_values.get(this_prov, def.offsets[i]) += def.values[i];
	}
	void apply_nat_modifier(world_state& ws, nations::country_tag this_nation, const modifier_definition& def, float scale) {
		for(uint32_t i = 0; i < modifier_definition_size; ++i)
			ws.w.nation_s.modifier_values.get(this_nation, def.offsets[i]) += def.values[i] * scale;
	}
	void apply_prov_modifier(world_state& ws, provinces::province_tag this_prov, const modifier_definition& def, float scale) {
		for(uint32_t i = 0; i < modifier_definition_size; ++i)
			ws.w.province_s.modifier_values.get(this_prov, def.offsets[i]) += def.values[i] * scale;
	}
	

	void reset_national_modifier(world_state& ws, nations::country_tag this_nation) {
		
		//modifier_values = national_modifier_vector::Zero();

		auto static_range = get_range(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(this_nation));
		for(auto m : static_range)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[m]);

		auto timed_modifiers = ws.w.nation_s.nations.get<nation::timed_modifiers>(this_nation);
		remove_item_if(ws.w.nation_s.timed_modifier_arrays, timed_modifiers,
			[d = to_index(ws.w.current_date) + 1](nations::timed_national_modifier const& m) { return to_index(m.expiration) <= d; });
		auto timed_range = get_range(ws.w.nation_s.timed_modifier_arrays, timed_modifiers);
		for(auto t = timed_range.first; t != timed_range.second; ++t)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[t->mod]);

		if(auto tech_school = ws.w.nation_s.nations.get<nation::tech_school>(this_nation); is_valid_index(tech_school))
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[tech_school]);
		if(auto national_value = ws.w.nation_s.nations.get<nation::national_value>(this_nation); is_valid_index(national_value))
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[national_value]);

		if(ws.w.nation_s.nations.get<nation::is_civilized>(this_nation) == false)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.unciv_nation]);
		else if(is_great_power(ws, this_nation))
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.great_power]);
		else if(ws.w.nation_s.nations.get<nation::overall_rank>(this_nation) <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.second_power]);
		else
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.civ_nation]);

		auto set_options = ws.w.nation_s.active_issue_options.get_row(this_nation);
		for(int32_t i = int32_t(ws.s.issues_m.issues_container.size()); i--; ) {
			auto active_option = set_options[i];
			if(is_valid_index(active_option) && is_valid_index(ws.s.issues_m.options[active_option].modifier))
				apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.issues_m.options[active_option].modifier]);
		}
		auto active_techs = ws.w.nation_s.active_technologies.get_row(this_nation);
		for(int32_t i = int32_t(ws.s.technology_m.technologies_container.size()); i--; ) {
			technologies::tech_tag tag(static_cast<technologies::tech_tag::value_base_t>(i));
			if(bit_vector_test(active_techs, uint32_t(i)) &&
				is_valid_index(ws.s.technology_m.technologies_container[tag].modifier))
				apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.technology_m.technologies_container[tag].modifier]);
		}

		bool at_war = get_size(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(this_nation)) != 0;
		auto central_province_count = ws.w.nation_s.nations.get<nation::central_province_count>(this_nation);

		if(at_war) {
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.war]);
		} else {
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.peace]);
			if(auto rebel_controlled_provinces = ws.w.nation_s.nations.get<nation::rebel_controlled_provinces>(this_nation) != 0 && central_province_count != 0) {
				apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.total_occupation],
					(float(rebel_controlled_provinces) / float(central_province_count)));
			}
		}
		
		if(auto infmay = ws.w.nation_s.nations.get<nation::infamy>(this_nation); infmay != 0.0f)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.badboy], 
				infmay / ws.s.modifiers_m.global_defines.badboy_limit);
		if(auto blockaded_count = ws.w.nation_s.nations.get<nation::blockaded_count>(this_nation); blockaded_count != 0 && central_province_count != 0)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.total_blockaded],
				(float(blockaded_count) / float(central_province_count)));
		if(auto war_exhaustion = ws.w.nation_s.nations.get<nation::war_exhaustion>(this_nation); war_exhaustion != 0.0f)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.war_exhaustion],
				100.0f * war_exhaustion);
		if(auto plurality = ws.w.nation_s.nations.get<nation::plurality>(this_nation); plurality != 0.0f)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.plurality], 100.0f * plurality);
		if(auto disarm_date = ws.w.nation_s.nations.get<nation::disarmed_until>(this_nation);
			is_valid_index(disarm_date) & (ws.w.current_date < disarm_date)) {
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.disarming]);
		}

		auto total_pop = ws.w.nation_s.nation_demographics.get(this_nation, population::total_population_tag);
		auto literacy = ws.w.nation_s.nation_demographics.get(this_nation, population::literacy_demo_tag(ws));
		if(total_pop != 0 && literacy != 0)
			apply_nat_modifier(ws, this_nation, ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.average_literacy], (float(literacy) / float(total_pop)));

		//if((this_nation.flags & nations::nation::is_bankrupt) != 0) {
		//	this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.in_bankrupcy];
		//	if(is_valid_index(ws.s.modifiers_m.static_modifiers.generalised_debt_default))
		//		this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.generalised_debt_default];
		//}
	}
	void reset_provincial_modifier(world_state& ws, provinces::province_tag this_province) {
		auto& container = ws.w.province_s.province_state_container;
		

		auto static_range = get_range(ws.w.province_s.static_modifier_arrays, container.get<province_state::static_modifiers>(this_province));
		for(auto m : static_range)
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[m]);

		auto& timed_array = container.get<province_state::timed_modifiers>(this_province);
		remove_item_if(ws.w.province_s.timed_modifier_arrays, timed_array,
			[d = to_index(ws.w.current_date) + 1](provinces::timed_provincial_modifier const& m) { return to_index(m.expiration) <= d; });
		auto timed_range = get_range(ws.w.province_s.timed_modifier_arrays, timed_array);
		for(auto t = timed_range.first; t != timed_range.second; ++t)
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[t->mod]);

		if(auto terrain = container.get<province_state::terrain>(this_province); is_valid_index(terrain))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[terrain]);
		if(auto climate = ws.s.province_m.province_container.get<province::climate>(this_province); is_valid_index(climate))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[climate]);
		if(auto continent = ws.s.province_m.province_container.get<province::continent>(this_province); is_valid_index(continent))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[continent]);

		if(auto crime = container.get<province_state::crime>(this_province); is_valid_index(crime))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[crime]);
		if(auto si = container.get<province_state::state_instance>(this_province); is_valid_index(si)) {
			if(auto nf = ws.w.nation_s.states.get<state::owner_national_focus>(si); is_valid_index(nf) && is_valid_index(ws.s.modifiers_m.national_focuses[nf].modifier))
				apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.national_focuses[nf].modifier]);
		}

		apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.infrastructure],
			(ws.s.economy_m.railroad.infrastructure * float(container.get<province_state::railroad_level>(this_province))));

		if(is_valid_index(ws.s.economy_m.fort_modifier))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.economy_m.fort_modifier],
				float(container.get<province_state::fort_level>(this_province)));
		if(is_valid_index(ws.s.economy_m.naval_base_modifier))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.economy_m.naval_base_modifier],
				float(container.get<province_state::naval_base_level>(this_province)));
		if(is_valid_index(ws.s.economy_m.railroad_modifier))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.economy_m.railroad_modifier],
				float(container.get<province_state::railroad_level>(this_province)));

		if(container.get<province_state::siege_progress>(this_province) != 0.0f)
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.has_siege]);
		if(container.get<province_state::is_overseas>(this_province))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.overseas]);
		if(container.get<province_state::is_blockaded>(this_province))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.blockaded]);
		if(container.get<province_state::has_owner_core>(this_province))
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.core]);

		if(ws.s.province_m.province_container.get<province::is_sea>(this_province)) {
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.sea_zone]);
			if(ws.s.province_m.province_container.get<province::is_coastal>(this_province))
				apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.coastal_sea]);
			else
				apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.non_coastal]);
		} else {
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.land_province]);
			if(ws.s.province_m.province_container.get<province::is_coastal>(this_province))
				apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.coastal]);
			else
				apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.non_coastal]);
		}

		if(auto nationalism = container.get<province_state::nationalism>(this_province); nationalism > 0.0f)
			apply_prov_modifier(ws, this_province, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.nationalism], nationalism);
	}

	void add_timed_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod, date_tag expiration) {
		add_item(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(this_nation), nations::timed_national_modifier{ expiration, mod });
	}
	void add_unique_timed_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod, date_tag expiration) {
		auto& timed = ws.w.nation_s.nations.get<nation::timed_modifiers>(this_nation);
		auto found = find(ws.w.nation_s.timed_modifier_arrays, timed, nations::timed_national_modifier{ expiration, mod });
		if(found)
			found->expiration = date_tag(std::max(to_index(found->expiration), to_index(expiration)));
		else
			add_item(ws.w.nation_s.timed_modifier_arrays, timed, nations::timed_national_modifier{ expiration, mod });
	}
	void add_timed_modifier_to_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod, date_tag expiration) {
		add_item(ws.w.province_s.timed_modifier_arrays,
			ws.w.province_s.province_state_container.get<province_state::timed_modifiers>(this_province),
			provinces::timed_provincial_modifier{ expiration, mod });

		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(this_province); 
			is_valid_index(nat_mod) && is_valid_index(owner)) {

			add_timed_modifier_to_nation(ws, owner, nat_mod, expiration);
		}
	}
	void remove_static_modifier_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod) {
		remove_single_item(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(this_nation), mod);
	}
	void remove_static_modifier_from_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod) {
		remove_item(ws.w.province_s.static_modifier_arrays,
			ws.w.province_s.province_state_container.get<province_state::static_modifiers>(this_province),
			mod);

		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(this_province);
			is_valid_index(nat_mod) && is_valid_index(owner)) {

			remove_static_modifier_from_nation(ws, owner, nat_mod);
		}
	}
	void remove_timed_modifier_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod, date_tag expiration) {
		remove_single_item(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(this_nation), nations::timed_national_modifier{ expiration, mod });
	}
	void remove_timed_modifier_from_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod, date_tag expiration) {
		remove_single_item(ws.w.province_s.timed_modifier_arrays,
			ws.w.province_s.province_state_container.get<province_state::timed_modifiers>(this_province),
			provinces::timed_provincial_modifier{ expiration, mod });

		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(this_province);
			is_valid_index(nat_mod) && is_valid_index(owner)) {

			remove_timed_modifier_from_nation(ws, owner, nat_mod, expiration);
		}
	}

	void remove_all_static_modifiers_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod) {
		remove_item(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(this_nation), mod);
	}
	void remove_all_timed_modifiers_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod) {
		remove_subrange(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(this_nation), nations::timed_national_modifier{ date_tag(), mod });
	}
	void remove_all_timed_modifiers_from_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod) {
		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		auto& timed_array = ws.w.province_s.province_state_container.get<province_state::timed_modifiers>(this_province);

		if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(this_province);
			is_valid_index(nat_mod) && is_valid_index(owner)) {

			auto nm_range = get_subrange(ws.w.province_s.timed_modifier_arrays,
				timed_array,
				provinces::timed_provincial_modifier{ date_tag(), mod });

			for(auto nm = nm_range.first; nm != nm_range.second; ++nm) {
				remove_timed_modifier_from_nation(ws, owner, nat_mod, nm->expiration);
			}
		}
		remove_subrange(ws.w.province_s.timed_modifier_arrays, timed_array, provinces::timed_provincial_modifier{ date_tag(), mod });
	}

	void detach_province_modifiers(world_state& ws, provinces::province_tag this_province, nations::country_tag nation_from) {
		auto smod = get_range(ws.w.province_s.static_modifier_arrays, ws.w.province_s.province_state_container.get<province_state::static_modifiers>(this_province));
		auto& n_timed = ws.w.nation_s.nations.get<nation::timed_modifiers>(nation_from);
		auto& n_static = ws.w.nation_s.nations.get<nation::static_modifiers>(nation_from);

		for(auto m : smod) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m].complement; is_valid_index(nm)) {
				remove_single_item(ws.w.nation_s.static_modifier_arrays, n_static, nm);
				apply_nat_modifier(ws, nation_from, ws.s.modifiers_m.national_modifier_definitions[nm], -1.0f);
			}
		}

		auto tmod = get_range(ws.w.province_s.timed_modifier_arrays, ws.w.province_s.province_state_container.get<province_state::timed_modifiers>(this_province));
		for(auto m = tmod.first; m != tmod.second; ++m) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m->mod].complement; is_valid_index(nm)) {
				remove_single_item(ws.w.nation_s.timed_modifier_arrays, n_timed, nations::timed_national_modifier{ m->expiration, nm });
				apply_nat_modifier(ws, nation_from, ws.s.modifiers_m.national_modifier_definitions[nm], -1.0f);
			}
		}
	}
	void attach_province_modifiers(world_state& ws, provinces::province_tag this_province, nations::country_tag nation_to) {
		auto smod = get_range(ws.w.province_s.static_modifier_arrays, ws.w.province_s.province_state_container.get<province_state::static_modifiers>(this_province));
		auto& n_timed = ws.w.nation_s.nations.get<nation::timed_modifiers>(nation_to);
		auto& n_static = ws.w.nation_s.nations.get<nation::static_modifiers>(nation_to);

		for(auto m : smod) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m].complement; is_valid_index(nm)) {
				add_item(ws.w.nation_s.static_modifier_arrays, n_static, nm);
				apply_nat_modifier(ws, nation_to, ws.s.modifiers_m.national_modifier_definitions[nm]);
			}
		}

		auto tmod = get_range(ws.w.province_s.timed_modifier_arrays, ws.w.province_s.province_state_container.get<province_state::timed_modifiers>(this_province));
		for(auto m = tmod.first; m != tmod.second; ++m) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m->mod].complement; is_valid_index(nm)) {
				add_item(ws.w.nation_s.timed_modifier_arrays, n_timed, nations::timed_national_modifier{ m->expiration, nm });
				apply_nat_modifier(ws, nation_to, ws.s.modifiers_m.national_modifier_definitions[nm]);
			}
		}
	}

	bool has_provincial_modifier(world_state const& ws, provinces::province_tag this_province, provincial_modifier_tag mod) {
		return contains_item(ws.w.province_s.static_modifier_arrays, ws.w.province_s.province_state_container.get<province_state::static_modifiers>(this_province), mod) ||
			contains_item(ws.w.province_s.timed_modifier_arrays, ws.w.province_s.province_state_container.get<province_state::timed_modifiers>(this_province), provinces::timed_provincial_modifier{ date_tag(), mod });
	}
	bool has_national_modifier(world_state const& ws, nations::country_tag this_nation, national_modifier_tag mod) {
		return contains_item(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(this_nation), mod) ||
			contains_item(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(this_nation), nations::timed_national_modifier{ date_tag(), mod });
	}

	float test_additive_factor(factor_tag t, world_state& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot, population::rebel_faction* rebel_slot) {
		return test_additive_factor(ws.s.modifiers_m.factor_modifiers[t], ws, primary_slot, from_slot, rebel_slot);
	}

	float test_additive_factor(factor_modifier const& f, world_state& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot, population::rebel_faction* rebel_slot) {
		float accumulated = f.factor;
		for(uint32_t i = 0; i < f.data_length; ++i) {
			auto segment = ws.s.modifiers_m.factor_data[f.data_offset + i];
			if(triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(segment.condition), ws, primary_slot, primary_slot, from_slot, rebel_slot))
				accumulated += segment.factor;
		}
		return accumulated;
	}

	float test_multiplicative_factor(factor_tag t, world_state& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot, population::rebel_faction* rebel_slot) {
		return test_multiplicative_factor(ws.s.modifiers_m.factor_modifiers[t], ws, primary_slot, from_slot, rebel_slot);
	}

	float test_multiplicative_factor(factor_modifier const& f, world_state& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot, population::rebel_faction* rebel_slot) {
		float accumulated = f.factor;
		for(uint32_t i = 0; i < f.data_length; ++i) {
			auto segment = ws.s.modifiers_m.factor_data[f.data_offset + i];
			if(triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(segment.condition), ws, primary_slot, primary_slot, from_slot, rebel_slot))
				accumulated *= segment.factor;
		}
		return accumulated;
	}

	int32_t maximum_national_focuses(world_state const& ws, nations::country_tag this_nation) {
		auto nation_id = this_nation;

		if(!ws.w.nation_s.nations.is_valid_index(nation_id))
			return 0;

		auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(this_nation);
		auto accepted_cultures = get_range(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(this_nation));
		float total_pop = 0;

		if(is_valid_index(prim_culture))
			total_pop += ws.w.nation_s.nation_demographics.get(nation_id, population::to_demo_tag(ws, prim_culture));
		for(auto ac : accepted_cultures) {
			if(is_valid_index(ac))
				total_pop += ws.w.nation_s.nation_demographics.get(nation_id, population::to_demo_tag(ws, ac));
		}

		float focus_amount = float(total_pop) / ws.s.modifiers_m.global_defines.national_focus_divider;
		float limited_amount = std::min(focus_amount, ws.w.nation_s.nations.get<nation::tech_attributes>(this_nation)[technologies::tech_offset::max_national_focus] + 1.0f);
		return std::max(int32_t(limited_amount), 1);
	}

	float extract_value_from_definition(int32_t offset, modifier_definition const& def) {
		for(uint32_t i = 0; i < modifier_definition_size; ++i) {
			if(def.offsets[i] == offset)
				return def.values[i];
		}
		return 0.0f;
	}
}
