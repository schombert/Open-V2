#include "common\\common.h"
#include "modifier_functions.h"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.h"

namespace modifiers {
	void add_unique_static_modifier_to_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod) {
		if(contains_item(ws.w.nation_s.static_modifier_arrays, this_nation.static_modifiers, mod) == false)
			add_item(ws.w.nation_s.static_modifier_arrays, this_nation.static_modifiers, mod);
	}
	void add_static_modifier_to_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod) {
		add_item(ws.w.nation_s.static_modifier_arrays, this_nation.static_modifiers, mod);
	}
	void add_static_modifier_to_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod) {
		add_item(ws.w.province_s.static_modifier_arrays, this_province.static_modifiers, mod);
		
		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(is_valid_index(nat_mod) && this_province.owner) {
			add_static_modifier_to_nation(ws, *this_province.owner, nat_mod);
		}
	}

	void set_default_crimes(world_state const& ws, nations::nation& this_nation) {
		this_nation.enabled_crimes = 0ui64;
		for(int32_t i = int32_t(ws.s.modifiers_m.crimes.size()); i--; ) {
			if(ws.s.modifiers_m.crimes[uint32_t(i)].default_active)
				bit_vector_set(&(this_nation.enabled_crimes), uint32_t(i), true);
		}
	}
	void reset_national_modifier(world_state const& ws, nations::nation& this_nation) {
		this_nation.modifier_values = national_modifier_vector::Zero();

		auto static_range = get_range(ws.w.nation_s.static_modifier_arrays, this_nation.static_modifiers);
		for(auto m : static_range)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[m];

		auto timed_range = get_range(ws.w.nation_s.timed_modifier_arrays, this_nation.timed_modifiers);
		for(auto t = timed_range.first; t != timed_range.second; ++t)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[t->mod];

		if(is_valid_index(this_nation.tech_school))
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[this_nation.tech_school];
		if(is_valid_index(this_nation.national_value))
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[this_nation.national_value];

		if((this_nation.flags & nations::nation::is_civilized) == 0)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.unciv_nation];
		else if(is_great_power(ws, this_nation))
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.great_power];
		else if(this_nation.overall_rank <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.second_power];
		else
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.civ_nation];

		auto set_options = ws.w.nation_s.active_issue_options.get_row(this_nation.id);
		for(int32_t i = int32_t(ws.s.issues_m.issues_container.size()); i--; ) {
			auto active_option = set_options[i];
			if(is_valid_index(active_option) && is_valid_index(ws.s.issues_m.options[active_option].modifier))
				this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.issues_m.options[active_option].modifier];
		}
		auto active_techs = ws.w.nation_s.active_technologies.get_row(this_nation.id);
		for(int32_t i = int32_t(ws.s.technology_m.technologies_container.size()); i--; ) {
			technologies::tech_tag tag(static_cast<technologies::tech_tag::value_base_t>(i));
			if(bit_vector_test(active_techs, uint32_t(i)) &&
				is_valid_index(ws.s.technology_m.technologies_container[tag].modifier))
				this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.technology_m.technologies_container[tag].modifier];
		}

		bool at_war = get_size(ws.w.military_s.war_arrays, this_nation.wars_involved_in) != 0;

		if(at_war) {
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.war];
		} else {
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.peace];
			if(this_nation.rebel_controlled_provinces != 0 && this_nation.central_province_count != 0)
				this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.total_occupation] * (float(this_nation.rebel_controlled_provinces) / float(this_nation.central_province_count));
		}
		
		if(this_nation.infamy != 0.0f)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.badboy] * this_nation.infamy / ws.s.modifiers_m.global_defines.badboy_limit;
		if(this_nation.blockaded_count != 0 && this_nation.central_province_count != 0)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.total_blockaded] * (float(this_nation.blockaded_count) / float(this_nation.central_province_count));
		if(this_nation.war_exhaustion != 0.0f)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.war_exhaustion] * 100.0f * this_nation.war_exhaustion;
		if(this_nation.plurality != 0.0f)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.plurality] * 100.0f * this_nation.plurality;
		if(is_valid_index(this_nation.disarmed_until) & (ws.w.current_date < this_nation.disarmed_until))
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.disarming];

		auto total_pop = ws.w.nation_s.nation_demographics.get(this_nation.id, population::total_population_tag);
		auto literacy = ws.w.nation_s.nation_demographics.get(this_nation.id, population::literacy_demo_tag(ws));
		if(total_pop != 0 && literacy != 0)
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.average_literacy] * (float(literacy) / float(total_pop));

		if((this_nation.flags & nations::nation::is_bankrupt) != 0) {
			this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.in_bankrupcy];
			if(is_valid_index(ws.s.modifiers_m.static_modifiers.generalised_debt_default))
				this_nation.modifier_values += ws.s.modifiers_m.national_modifier_definitions[ws.s.modifiers_m.static_modifiers.generalised_debt_default];
		}
	}
	void reset_provincial_modifier(world_state const& ws, provinces::province_state& this_province) {
		this_province.modifier_values = provincial_modifier_vector::Zero();
		auto& base_province = ws.s.province_m.province_container[this_province.id];

		auto static_range = get_range(ws.w.province_s.static_modifier_arrays, this_province.static_modifiers);
		for(auto m : static_range)
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[m];

		auto timed_range = get_range(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers);
		for(auto t = timed_range.first; t != timed_range.second; ++t)
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[t->mod];

		if(is_valid_index(this_province.terrain))
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[this_province.terrain];
		if(is_valid_index(base_province.climate))
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[base_province.climate];
		if(is_valid_index(base_province.continent))
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[base_province.continent];

		if(is_valid_index(this_province.crime))
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[this_province.crime];
		if(this_province.state_instance) {
			if(auto nf = this_province.state_instance->owner_national_focus; nf && is_valid_index(nf->modifier))
				this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[nf->modifier];
		}

		this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.infrastructure] *
			(ws.s.economy_m.railroad.infrastructure * float(this_province.railroad_level));

		if(this_province.siege_progress != 0.0f)
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.has_siege];
		if((this_province.flags & provinces::province_state::is_overseas) != 0)
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.overseas];
		if((this_province.flags & provinces::province_state::is_blockaded) != 0)
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.blockaded];
		if((this_province.flags & provinces::province_state::has_owner_core) != 0)
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.core];

		if((base_province.flags & provinces::province::sea) != 0) {
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.sea_zone];
			if((base_province.flags & provinces::province::coastal) != 0)
				this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.coastal_sea];
			else
				this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.non_coastal];
		} else {
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.land_province];
			if((base_province.flags & provinces::province::coastal) != 0)
				this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.coastal];
			else
				this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.non_coastal];
		}

		if(this_province.nationalism != 0.0f)
			this_province.modifier_values += ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.static_modifiers.nationalism] * this_province.nationalism;
	}

	void add_timed_modifier_to_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod, date_tag expiration) {
		add_item(ws.w.nation_s.timed_modifier_arrays, this_nation.timed_modifiers, nations::timed_national_modifier{ expiration, mod });
	}
	void add_timed_modifier_to_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod, date_tag expiration) {
		add_item(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers, provinces::timed_provincial_modifier{ expiration, mod });

		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(is_valid_index(nat_mod) && this_province.owner) {
			add_timed_modifier_to_nation(ws, *this_province.owner, nat_mod, expiration);
		}
	}
	void remove_static_modifier_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod) {
		remove_single_item(ws.w.nation_s.static_modifier_arrays, this_nation.static_modifiers, mod);
	}
	void remove_static_modifier_from_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod) {
		remove_item(ws.w.province_s.static_modifier_arrays, this_province.static_modifiers, mod);

		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(is_valid_index(nat_mod) && this_province.owner) {
			remove_static_modifier_from_nation(ws, *this_province.owner, nat_mod);
		}
	}
	void remove_timed_modifier_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod, date_tag expiration) {
		remove_single_item(ws.w.nation_s.timed_modifier_arrays, this_nation.timed_modifiers, nations::timed_national_modifier{ expiration, mod });
	}
	void remove_timed_modifier_from_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod, date_tag expiration) {
		remove_single_item(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers, provinces::timed_provincial_modifier{ expiration, mod });

		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(is_valid_index(nat_mod) && this_province.owner) {
			remove_timed_modifier_from_nation(ws, *this_province.owner, nat_mod, expiration);
		}
	}

	void remove_all_static_modifiers_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod) {
		remove_item(ws.w.nation_s.static_modifier_arrays, this_nation.static_modifiers, mod);
	}
	void remove_all_timed_modifiers_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod) {
		remove_subrange(ws.w.nation_s.timed_modifier_arrays, this_nation.timed_modifiers, nations::timed_national_modifier{ date_tag(), mod });
	}
	void remove_all_timed_modifiers_from_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod) {
		auto nat_mod = ws.s.modifiers_m.provincial_modifiers[mod].complement;
		if(is_valid_index(nat_mod) && this_province.owner) {
			auto nm_range = get_subrange(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers, provinces::timed_provincial_modifier{ date_tag(), mod });
			for(auto nm = nm_range.first; nm != nm_range.second; ++nm) {
				remove_timed_modifier_from_nation(ws, *this_province.owner, nat_mod, nm->expiration);
			}
		}
		remove_subrange(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers, provinces::timed_provincial_modifier{ date_tag(), mod });
	}

	void detach_province_modifiers(world_state& ws, provinces::province_state& this_province, nations::nation& nation_from) {
		auto smod = get_range(ws.w.province_s.static_modifier_arrays, this_province.static_modifiers);
		for(auto m : smod) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m].complement; is_valid_index(nm)) {
				remove_single_item(ws.w.nation_s.static_modifier_arrays, nation_from.static_modifiers, nm);
				nation_from.modifier_values -= ws.s.modifiers_m.national_modifier_definitions[nm];
			}
		}

		auto tmod = get_range(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers);
		for(auto m = tmod.first; m != tmod.second; ++m) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m->mod].complement; is_valid_index(nm)) {
				remove_single_item(ws.w.nation_s.timed_modifier_arrays, nation_from.timed_modifiers, nations::timed_national_modifier{ m->expiration, nm });
				nation_from.modifier_values -= ws.s.modifiers_m.national_modifier_definitions[nm];
			}
		}
	}
	void attach_province_modifiers(world_state& ws, provinces::province_state& this_province, nations::nation& nation_to) {
		auto smod = get_range(ws.w.province_s.static_modifier_arrays, this_province.static_modifiers);
		for(auto m : smod) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m].complement; is_valid_index(nm)) {
				add_item(ws.w.nation_s.static_modifier_arrays, nation_to.static_modifiers, nm);
				nation_to.modifier_values += ws.s.modifiers_m.national_modifier_definitions[nm];
			}
		}

		auto tmod = get_range(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers);
		for(auto m = tmod.first; m != tmod.second; ++m) {
			if(auto nm = ws.s.modifiers_m.provincial_modifiers[m->mod].complement; is_valid_index(nm)) {
				add_item(ws.w.nation_s.timed_modifier_arrays, nation_to.timed_modifiers, nations::timed_national_modifier{ m->expiration, nm });
				nation_to.modifier_values += ws.s.modifiers_m.national_modifier_definitions[nm];
			}
		}
	}

	bool has_provincial_modifier(world_state const& ws, provinces::province_state const& this_province, provincial_modifier_tag mod) {
		return contains_item(ws.w.province_s.static_modifier_arrays, this_province.static_modifiers, mod) ||
			contains_item(ws.w.province_s.timed_modifier_arrays, this_province.timed_modifiers, provinces::timed_provincial_modifier{ date_tag(), mod });
	}
	bool has_national_modifier(world_state const& ws, nations::nation const& this_nation, national_modifier_tag mod) {
		return contains_item(ws.w.nation_s.static_modifier_arrays, this_nation.static_modifiers, mod) ||
			contains_item(ws.w.nation_s.timed_modifier_arrays, this_nation.timed_modifiers, nations::timed_national_modifier{ date_tag(), mod });
	}

	float test_multiplicative_factor(factor_tag t, world_state& ws, void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return test_multiplicative_factor(ws.s.modifiers_m.factor_modifiers[t], ws, primary_slot, from_slot, rebel_slot);
	}

	float test_multiplicative_factor(factor_modifier const& f, world_state& ws, void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		float accumulated = f.factor;
		for(uint32_t i = 0; i < f.data_length; ++i) {
			auto segment = ws.s.modifiers_m.factor_data[f.data_offset + i];
			if(triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(segment.condition), ws, primary_slot, primary_slot, from_slot, rebel_slot))
				accumulated *= segment.factor;
		}
		return accumulated;
	}

	int32_t maximum_national_focuses(world_state const& ws, nations::nation const& this_nation) {
		auto nation_id = this_nation.id;

		if(!ws.w.nation_s.nations.is_valid_index(nation_id))
			return 0;

		auto prim_culture = this_nation.primary_culture;
		auto accepted_cultures = get_range(ws.w.culture_s.culture_arrays, this_nation.accepted_cultures);
		int64_t total_pop = 0;

		if(is_valid_index(prim_culture))
			total_pop += ws.w.nation_s.nation_demographics.get(nation_id, population::to_demo_tag(ws, prim_culture));
		for(auto ac : accepted_cultures) {
			if(is_valid_index(ac))
				total_pop += ws.w.nation_s.nation_demographics.get(nation_id, population::to_demo_tag(ws, ac));
		}

		float focus_amount = float(total_pop) / ws.s.modifiers_m.global_defines.national_focus_divider;
		float limited_amount = std::min(focus_amount, this_nation.tech_attributes[technologies::tech_offset::max_national_focus] + 1.0f);
		return std::max(int32_t(limited_amount), 1);
	}
}
