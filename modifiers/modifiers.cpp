#include "modifiers.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"
#include "triggers\\trigger_reading.h"
#include "scenario\\scenario.h"

namespace modifiers {
	struct parsing_environment {
		text_handle_lookup text_lookup;

		modifiers_manager& manager;

		parsed_data crimes_file;
		parsed_data nv_file;

		parsing_environment(const text_handle_lookup& tl, modifiers_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, modifiers_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct empty_type {
		void add_unknown_key(int) {
		}
	};

	struct crimes_preparse_file {
		parsing_environment& env;
		crimes_preparse_file(parsing_environment& e) : env(e) {}

		void add_crime(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			const auto tag = env.manager.provincial_modifiers.emplace_back();
			auto& new_i = env.manager.provincial_modifiers[tag];

			new_i.id = tag;
			new_i.name = name;

			env.manager.named_provincial_modifiers_index.emplace(name, tag);
		}
	};

	struct nv_preparse_file {
		parsing_environment& env;
		nv_preparse_file(parsing_environment& e) : env(e) {}

		void add_nv(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			const auto tag = env.manager.national_modifiers.emplace_back();
			auto& new_i = env.manager.national_modifiers[tag];

			new_i.id = tag;
			new_i.name = name;

			env.manager.named_national_modifiers_index.emplace(name, tag);
		}
	};

	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }
	inline token_and_type name_empty_type(const token_and_type& t, association_type, empty_type&) { return t; }

	std::pair<uint32_t, uint32_t> get_provincial_and_national_offsets_from_token(const char* s, const char* e) {
		using token_tree_unsorted = type_list<
			CT_STRING_INT("supply_limit", pack_offset_pair(provincial_offsets::supply_limit, bad_offset)),
			CT_STRING_INT("attrition", pack_offset_pair(provincial_offsets::attrition, bad_offset)),
			CT_STRING_INT("max_attrition", pack_offset_pair(provincial_offsets::max_attrition, bad_offset)),
			CT_STRING_INT("war_exhaustion", pack_offset_pair(bad_offset, national_offsets::war_exhaustion)),
			CT_STRING_INT("max_war_exhaustion", pack_offset_pair(bad_offset, national_offsets::max_war_exhaustion)),
			CT_STRING_INT("leadership", pack_offset_pair(bad_offset, national_offsets::leadership)),
			CT_STRING_INT("leadership_modifier", pack_offset_pair(bad_offset, national_offsets::leadership_modifier)),
			CT_STRING_INT("supply_consumption", pack_offset_pair(bad_offset, national_offsets::supply_consumption)),
			CT_STRING_INT("org_regain", pack_offset_pair(bad_offset, national_offsets::org_regain)),
			CT_STRING_INT("reinforce_speed", pack_offset_pair(bad_offset, national_offsets::reinforce_speed)),
			CT_STRING_INT("land_organisation", pack_offset_pair(bad_offset, national_offsets::land_organisation)),
			CT_STRING_INT("naval_organisation", pack_offset_pair(bad_offset, national_offsets::naval_organisation)),
			CT_STRING_INT("research_points", pack_offset_pair(bad_offset, national_offsets::research_points)),
			CT_STRING_INT("research_points_modifier", pack_offset_pair(bad_offset, national_offsets::research_points_modifier)),
			CT_STRING_INT("research_points_on_conquer", pack_offset_pair(bad_offset, national_offsets::research_points_on_conquer)),
			CT_STRING_INT("import_cost", pack_offset_pair(bad_offset, national_offsets::import_cost)),
			CT_STRING_INT("loan_interest", pack_offset_pair(bad_offset, national_offsets::loan_interest)),
			CT_STRING_INT("tax_efficiency", pack_offset_pair(bad_offset, national_offsets::tax_efficiency)),
			CT_STRING_INT("min_tax", pack_offset_pair(bad_offset, national_offsets::min_tax)),
			CT_STRING_INT("max_tax", pack_offset_pair(bad_offset, national_offsets::max_tax)),
			CT_STRING_INT("min_military_spending", pack_offset_pair(bad_offset, national_offsets::min_military_spending)),
			CT_STRING_INT("max_military_spending", pack_offset_pair(bad_offset, national_offsets::max_military_spending)),
			CT_STRING_INT("min_social_spending", pack_offset_pair(bad_offset, national_offsets::min_social_spending)),
			CT_STRING_INT("max_social_spending", pack_offset_pair(bad_offset, national_offsets::max_social_spending)),
			CT_STRING_INT("factory_owner_cost", pack_offset_pair(bad_offset, national_offsets::factory_owner_cost)),
			CT_STRING_INT("min_tariff", pack_offset_pair(bad_offset, national_offsets::min_tariff)),
			CT_STRING_INT("max_tariff", pack_offset_pair(bad_offset, national_offsets::max_tariff)),
			CT_STRING_INT("ruling_party_support", pack_offset_pair(bad_offset, national_offsets::ruling_party_support)),
			CT_STRING_INT("local_ruling_party_support", pack_offset_pair(provincial_offsets::local_ruling_party_support, bad_offset)),
			CT_STRING_INT("rich_vote", pack_offset_pair(bad_offset, national_offsets::rich_vote)),
			CT_STRING_INT("middle_vote", pack_offset_pair(bad_offset, national_offsets::middle_vote)),
			CT_STRING_INT("poor_vote", pack_offset_pair(bad_offset, national_offsets::poor_vote)),
			CT_STRING_INT("minimum_wage", pack_offset_pair(bad_offset, national_offsets::minimum_wage)),
			CT_STRING_INT("factory_maintenance", pack_offset_pair(bad_offset, national_offsets::factory_maintenance)),
			CT_STRING_INT("poor_life_needs", pack_offset_pair(provincial_offsets::poor_life_needs, national_offsets::poor_life_needs)),
			CT_STRING_INT("rich_life_needs", pack_offset_pair(provincial_offsets::rich_life_needs, national_offsets::rich_life_needs)),
			CT_STRING_INT("middle_life_needs", pack_offset_pair(provincial_offsets::middle_life_needs, national_offsets::middle_life_needs)),
			CT_STRING_INT("poor_everyday_needs", pack_offset_pair(provincial_offsets::poor_everyday_needs, national_offsets::poor_everyday_needs)),
			CT_STRING_INT("rich_everyday_needs", pack_offset_pair(provincial_offsets::rich_everyday_needs, national_offsets::rich_everyday_needs)),
			CT_STRING_INT("middle_everyday_needs", pack_offset_pair(provincial_offsets::middle_everyday_needs, national_offsets::middle_everyday_needs)),
			CT_STRING_INT("poor_luxury_needs", pack_offset_pair(provincial_offsets::poor_luxury_needs, national_offsets::poor_luxury_needs)),
			CT_STRING_INT("middle_luxury_needs", pack_offset_pair(provincial_offsets::middle_luxury_needs, national_offsets::middle_luxury_needs)),
			CT_STRING_INT("rich_luxury_needs", pack_offset_pair(provincial_offsets::rich_luxury_needs, national_offsets::rich_luxury_needs)),
			CT_STRING_INT("unemployment_benefit", pack_offset_pair(bad_offset, national_offsets::unemployment_benefit)),
			CT_STRING_INT("pension_level", pack_offset_pair(bad_offset, national_offsets::pension_level)),
			CT_STRING_INT("population_growth", pack_offset_pair(provincial_offsets::population_growth, bad_offset)),
			CT_STRING_INT("global_population_growth", pack_offset_pair(bad_offset, national_offsets::global_population_growth)),
			CT_STRING_INT("factory_input", pack_offset_pair(bad_offset, national_offsets::factory_input)),
			CT_STRING_INT("factory_output", pack_offset_pair(bad_offset, national_offsets::factory_output)),
			CT_STRING_INT("factory_throughput", pack_offset_pair(bad_offset, national_offsets::factory_throughput)),
			CT_STRING_INT("rgo_input", pack_offset_pair(bad_offset, national_offsets::rgo_input)),
			CT_STRING_INT("rgo_output", pack_offset_pair(bad_offset, national_offsets::rgo_output)),
			CT_STRING_INT("rgo_throughput", pack_offset_pair(bad_offset, national_offsets::rgo_throughput)),
			CT_STRING_INT("artisan_input", pack_offset_pair(bad_offset, national_offsets::artisan_input)),
			CT_STRING_INT("artisan_output", pack_offset_pair(bad_offset, national_offsets::artisan_output)),
			CT_STRING_INT("artisan_throughput", pack_offset_pair(bad_offset, national_offsets::artisan_throughput)),
			CT_STRING_INT("local_factory_input", pack_offset_pair(provincial_offsets::local_factory_input, bad_offset)),
			CT_STRING_INT("local_factory_output", pack_offset_pair(provincial_offsets::local_factory_output, bad_offset)),
			CT_STRING_INT("local_factory_throughput", pack_offset_pair(provincial_offsets::local_factory_throughput, bad_offset)),
			CT_STRING_INT("local_rgo_input", pack_offset_pair(provincial_offsets::local_rgo_input, bad_offset)),
			CT_STRING_INT("local_rgo_output", pack_offset_pair(provincial_offsets::local_rgo_output, bad_offset)),
			CT_STRING_INT("local_rgo_throughput", pack_offset_pair(provincial_offsets::local_rgo_throughput, bad_offset)),
			CT_STRING_INT("local_artisan_input", pack_offset_pair(provincial_offsets::local_artisan_input, bad_offset)),
			CT_STRING_INT("local_artisan_output", pack_offset_pair(provincial_offsets::local_artisan_output, bad_offset)),
			CT_STRING_INT("local_artisan_throughput", pack_offset_pair(provincial_offsets::local_artisan_throughput, bad_offset)),
			CT_STRING_INT("number_of_voters", pack_offset_pair(provincial_offsets::number_of_voters, bad_offset)),
			CT_STRING_INT("goods_demand", pack_offset_pair(provincial_offsets::goods_demand, national_offsets::goods_demand)),
			CT_STRING_INT("badboy", pack_offset_pair(bad_offset, national_offsets::badboy)),
			CT_STRING_INT("assimilation_rate", pack_offset_pair(provincial_offsets::assimilation_rate, bad_offset)),
			CT_STRING_INT("global_assimilation_rate", pack_offset_pair(bad_offset, national_offsets::global_assimilation_rate)),
			CT_STRING_INT("prestige", pack_offset_pair(bad_offset, national_offsets::prestige)),
			CT_STRING_INT("factory_cost", pack_offset_pair(bad_offset, national_offsets::factory_cost)),
			CT_STRING_INT("life_rating", pack_offset_pair(provincial_offsets::life_rating, bad_offset)),
			CT_STRING_INT("farm_rgo_eff", pack_offset_pair(provincial_offsets::farm_rgo_eff, national_offsets::farm_rgo_eff)),
			CT_STRING_INT("mine_rgo_eff", pack_offset_pair(provincial_offsets::mine_rgo_eff, national_offsets::mine_rgo_eff)),
			CT_STRING_INT("farm_rgo_size", pack_offset_pair(provincial_offsets::farm_rgo_size, national_offsets::farm_rgo_size)),
			CT_STRING_INT("mine_rgo_size", pack_offset_pair(provincial_offsets::mine_rgo_size, national_offsets::mine_rgo_size)),
			CT_STRING_INT("issue_change_speed", pack_offset_pair(bad_offset, national_offsets::issue_change_speed)),
			CT_STRING_INT("social_reform_desire", pack_offset_pair(bad_offset, national_offsets::social_reform_desire)),
			CT_STRING_INT("political_reform_desire", pack_offset_pair(bad_offset, national_offsets::political_reform_desire)),
			CT_STRING_INT("literacy_con_impact", pack_offset_pair(bad_offset, national_offsets::literacy_con_impact)),
			CT_STRING_INT("pop_militancy_modifier", pack_offset_pair(provincial_offsets::pop_militancy_modifier, bad_offset)),
			CT_STRING_INT("pop_consciousness_modifier", pack_offset_pair(provincial_offsets::pop_consciousness_modifier, bad_offset)),
			CT_STRING_INT("rich_income_modifier", pack_offset_pair(provincial_offsets::rich_income_modifier, national_offsets::rich_income_modifier)),
			CT_STRING_INT("middle_income_modifier", pack_offset_pair(provincial_offsets::middle_income_modifier, national_offsets::middle_income_modifier)),
			CT_STRING_INT("poor_income_modifier", pack_offset_pair(provincial_offsets::poor_income_modifier, national_offsets::poor_income_modifier)),
			CT_STRING_INT("boost_strongest_party", pack_offset_pair(provincial_offsets::boost_strongest_party, bad_offset)),
			CT_STRING_INT("global_immigrant_attract", pack_offset_pair(bad_offset, national_offsets::global_immigrant_attract)),
			CT_STRING_INT("immigrant_attract", pack_offset_pair(provincial_offsets::immigrant_attract, bad_offset)),
			CT_STRING_INT("immigrant_push", pack_offset_pair(provincial_offsets::immigrant_push, bad_offset)),
			CT_STRING_INT("local_repair", pack_offset_pair(provincial_offsets::local_repair, bad_offset)),
			CT_STRING_INT("local_ship_build", pack_offset_pair(provincial_offsets::local_ship_build, bad_offset)),
			CT_STRING_INT("poor_savings_modifier", pack_offset_pair(bad_offset, national_offsets::poor_savings_modifier)),
			CT_STRING_INT("influence_modifier", pack_offset_pair(bad_offset, national_offsets::influence_modifier)),
			CT_STRING_INT("diplomatic_points_modifier", pack_offset_pair(bad_offset, national_offsets::diplomatic_points_modifier)),
			CT_STRING_INT("mobilisation_size", pack_offset_pair(bad_offset, national_offsets::mobilisation_size)),
			CT_STRING_INT("mobilisation_economy_impact", pack_offset_pair(bad_offset, national_offsets::mobilisation_economy_impact)),
			CT_STRING_INT("global_pop_militancy_modifier", pack_offset_pair(bad_offset, national_offsets::global_pop_militancy_modifier)),
			CT_STRING_INT("global_pop_consciousness_modifier", pack_offset_pair(bad_offset, national_offsets::global_pop_consciousness_modifier)),
			CT_STRING_INT("core_pop_militancy_modifier", pack_offset_pair(bad_offset, national_offsets::core_pop_militancy_modifier)),
			CT_STRING_INT("core_pop_consciousness_modifier", pack_offset_pair(bad_offset, national_offsets::core_pop_consciousness_modifier)),
			CT_STRING_INT("non_accepted_pop_militancy_modifier", pack_offset_pair(bad_offset, national_offsets::non_accepted_pop_militancy_modifier)),
			CT_STRING_INT("non_accepted_pop_consciousness_modifier", pack_offset_pair(bad_offset, national_offsets::non_accepted_pop_consciousness_modifier)),
			CT_STRING_INT("cb_generation_speed_modifier", pack_offset_pair(bad_offset, national_offsets::cb_generation_speed_modifier)),
			CT_STRING_INT("mobilization_impact", pack_offset_pair(bad_offset, national_offsets::mobilization_impact)),
			CT_STRING_INT("suppression_points_modifier", pack_offset_pair(bad_offset, national_offsets::suppression_points_modifier)),
			CT_STRING_INT("education_efficiency_modifier", pack_offset_pair(bad_offset, national_offsets::education_efficiency_modifier)),
			CT_STRING_INT("civilization_progress_modifier", pack_offset_pair(bad_offset, national_offsets::civilization_progress_modifier)),
			CT_STRING_INT("administrative_efficiency_modifier", pack_offset_pair(bad_offset, national_offsets::administrative_efficiency_modifier)),
			CT_STRING_INT("land_unit_start_experience", pack_offset_pair(bad_offset, national_offsets::land_unit_start_experience)),
			CT_STRING_INT("naval_unit_start_experience", pack_offset_pair(bad_offset, national_offsets::naval_unit_start_experience)),
			CT_STRING_INT("naval_attack_modifier", pack_offset_pair(bad_offset, national_offsets::naval_attack_modifier)),
			CT_STRING_INT("naval_defense_modifier", pack_offset_pair(bad_offset, national_offsets::naval_defense_modifier)),
			CT_STRING_INT("land_attack_modifier", pack_offset_pair(bad_offset, national_offsets::land_attack_modifier)),
			CT_STRING_INT("land_defense_modifier", pack_offset_pair(bad_offset, national_offsets::land_defense_modifier)),
			CT_STRING_INT("tariff_efficiency_modifier", pack_offset_pair(bad_offset, national_offsets::tariff_efficiency_modifier)),
			CT_STRING_INT("max_loan_modifier", pack_offset_pair(bad_offset, national_offsets::max_loan_modifier)),
			CT_STRING_INT("movement_cost", pack_offset_pair(provincial_offsets::movement_cost, bad_offset)),
			CT_STRING_INT("unciv_economic_modifier", pack_offset_pair(bad_offset, national_offsets::unciv_economic_modifier)),
			CT_STRING_INT("unciv_military_modifier", pack_offset_pair(bad_offset, national_offsets::unciv_military_modifier)),
			CT_STRING_INT("self_unciv_economic_modifier", pack_offset_pair(bad_offset, national_offsets::self_unciv_economic_modifier)),
			CT_STRING_INT("self_unciv_military_modifier", pack_offset_pair(bad_offset, national_offsets::self_unciv_military_modifier)),
			CT_STRING_INT("defender", pack_offset_pair(provincial_offsets::defense, bad_offset)),
			CT_STRING_INT("attacker", pack_offset_pair(provincial_offsets::attack, bad_offset)),
			CT_STRING_INT("defence", pack_offset_pair(provincial_offsets::defense, bad_offset)),
			CT_STRING_INT("attack", pack_offset_pair(provincial_offsets::attack, bad_offset)),
			CT_STRING_INT("combat_width", pack_offset_pair(provincial_offsets::combat_width, bad_offset)),
			CT_STRING_INT("min_build_naval_base", pack_offset_pair(provincial_offsets::min_build_naval_base, bad_offset)),
			CT_STRING_INT("min_build_railroad", pack_offset_pair(provincial_offsets::min_build_railroad, bad_offset)),
			CT_STRING_INT("min_build_fort", pack_offset_pair(provincial_offsets::min_build_fort, bad_offset)),
			CT_STRING_INT("commerce_tech_research_bonus", pack_offset_pair(bad_offset, national_offsets::commerce_tech_research_bonus)),
			CT_STRING_INT("army_tech_research_bonus", pack_offset_pair(bad_offset, national_offsets::army_tech_research_bonus)),
			CT_STRING_INT("industry_tech_research_bonus", pack_offset_pair(bad_offset, national_offsets::industry_tech_research_bonus)),
			CT_STRING_INT("navy_tech_research_bonus", pack_offset_pair(bad_offset, national_offsets::navy_tech_research_bonus)),
			CT_STRING_INT("culture_tech_research_bonus", pack_offset_pair(bad_offset, national_offsets::culture_tech_research_bonus))
		>;
		using token_tree = typename sorted<token_tree_unsorted>::type;

		const auto value = bt_find_value_or<token_tree, uint32_t>(s, e, pack_offset_pair(bad_offset, bad_offset));
		return std::pair<uint32_t, uint32_t>(get_provincial_offset_from_packed(value), get_national_offset_from_packed(value));
	}

	provincial_modifier_tag add_provincial_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager) {
		if (mod.total_attributes == 0)
			return provincial_modifier_tag();

		const auto pmtag = manager.fetch_unique_provincial_modifier(name);
		auto& new_mod = manager.provincial_modifiers[pmtag];
		new_mod.icon = mod.icon;

		Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32> dest_vector(manager.provincial_modifier_definitions.safe_get_row(pmtag), provincial_offsets::aligned_32_size);
		Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32> source_vector(mod.modifier_data.data(), provincial_offsets::aligned_32_size);

		dest_vector = source_vector;

		if (mod.count_unique_national != 0) {
			mod.remove_shared_national_attributes();
			const auto nat_result = add_national_modifier(text_data::text_tag(), mod, manager); // creates an unindexed national modifier as complement
			new_mod.complement = nat_result;
			manager.national_modifiers[nat_result].name = name;
		}

		return pmtag;
	}
	national_modifier_tag add_national_modifier(text_data::text_tag name, const modifier_reading_base& mod, modifiers_manager& manager) {
		if (mod.total_attributes == 0)
			return national_modifier_tag();

		const auto pmtag = manager.fetch_unique_national_modifier(name);
		auto& new_mod = manager.national_modifiers[pmtag];
		new_mod.icon = mod.icon;

		Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32> dest_vector(manager.national_modifier_definitions.safe_get_row(pmtag), national_offsets::aligned_32_size);
		Eigen::Map<const Eigen::VectorXf, Eigen::AlignmentType::Aligned32> source_vector(mod.modifier_data.data() + provincial_offsets::aligned_32_size, national_offsets::aligned_32_size);

		dest_vector = source_vector;

		return pmtag;
	}
	void add_indeterminate_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager) {
		if (mod.total_attributes == 0)
			return;
		if (mod.count_unique_provincial != 0) {
			// some attributes apply only to province
			// conclude intent is provincial modifier with additional national properties
			add_provincial_modifier(name, mod, manager);
		} else if (mod.count_unique_national == mod.total_attributes) {
			// all attributes apply only to nation
			// conclude intent is national modifier
			add_national_modifier(name, mod, manager);
		} else {
			//intent indeterminate: create first a national modifier (non destructive), then destructively create provincial and complement
			add_national_modifier(name, mod, manager);
			add_provincial_modifier(name, mod, manager);
		}
	}

	void modifier_reading_base::add_attribute(const std::pair<token_and_type, float>& p) {
		const auto offsets = get_provincial_and_national_offsets_from_token(p.first.start, p.first.end);
		if (offsets.first != bad_offset) {
			modifier_data[offsets.first] = p.second;
			++count_unique_provincial;
		}
		if (offsets.second != bad_offset) {
			modifier_data[provincial_offsets::aligned_32_size + offsets.second] = p.second;
			++count_unique_national;
		}
		if ((offsets.first != bad_offset) & (offsets.second != bad_offset)) {
			--count_unique_provincial;
			--count_unique_national;
		}
		if ((offsets.first != bad_offset) | (offsets.second != bad_offset)) {
			++total_attributes;
		}
	}
	void modifier_reading_base::remove_shared_national_attributes() {
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::goods_demand] = 0.0f;

		modifier_data[provincial_offsets::aligned_32_size + national_offsets::poor_life_needs] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::rich_life_needs] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::middle_life_needs] = 0.0f;

		modifier_data[provincial_offsets::aligned_32_size + national_offsets::poor_luxury_needs] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::middle_luxury_needs] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::rich_luxury_needs] = 0.0f;

		modifier_data[provincial_offsets::aligned_32_size + national_offsets::poor_everyday_needs] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::rich_everyday_needs] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::middle_everyday_needs] = 0.0f;

		modifier_data[provincial_offsets::aligned_32_size + national_offsets::farm_rgo_eff] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::mine_rgo_eff] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::farm_rgo_size] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::mine_rgo_size] = 0.0f;

		modifier_data[provincial_offsets::aligned_32_size + national_offsets::rich_income_modifier] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::middle_income_modifier] = 0.0f;
		modifier_data[provincial_offsets::aligned_32_size + national_offsets::poor_income_modifier] = 0.0f;
	}

	national_modifier_tag modifiers_manager::fetch_unique_national_modifier(text_data::text_tag n) {
		const auto f = named_national_modifiers_index.find(n);
		if (f != named_national_modifiers_index.end())
			return f->second;
		const auto tag = national_modifiers.emplace_back();
		auto& nm = national_modifiers[tag];
		nm.id = tag;
		nm.name = n;
		if(is_valid_index(n))
			named_national_modifiers_index.emplace(n, tag);
		return tag;
	}

	provincial_modifier_tag modifiers_manager::fetch_unique_provincial_modifier(text_data::text_tag n) {
		const auto f = named_provincial_modifiers_index.find(n);
		if (f != named_provincial_modifiers_index.end())
			return f->second;
		const auto tag = provincial_modifiers.emplace_back();
		auto& nm = provincial_modifiers[tag];
		nm.id = tag;
		nm.name = n;
		if (is_valid_index(n))
			named_provincial_modifiers_index.emplace(n, tag);
		return tag;
	}

	std::pair<uint16_t, bool> commit_factor(modifiers_manager& m, const std::vector<factor_segment>& factor) {
		if (factor.size() == 0ui64)
			return std::pair<uint16_t, bool>(0ui16, false);

		const auto search_result = std::search(m.factor_data.begin(), m.factor_data.end(), factor.begin(), factor.end());
		if (search_result != m.factor_data.end()) {
			return std::pair<uint16_t, bool>(static_cast<uint16_t>(search_result - m.factor_data.begin()), false);
		} else {
			const auto new_start_pos = m.factor_data.size();
			m.factor_data.insert(m.factor_data.end(), factor.begin(), factor.end());
			return std::pair<uint16_t, bool>(static_cast<uint16_t>(new_start_pos), true);
		}
	}
	

	struct factor_modifiers_parsing_state {
		scenario::scenario_manager& s;
		std::vector<factor_segment> data;
		factor_modifier under_construction;
		const triggers::trigger_scope_state modifier_scope;

		factor_modifiers_parsing_state(scenario::scenario_manager& sm, triggers::trigger_scope_state scope) : s(sm), modifier_scope(scope) {}
	};

	inline triggers::trigger_and_factor read_factor_and_trigger(const token_group* s, const token_group* e, factor_modifiers_parsing_state& env) {
		return parse_trigger_and_factor(env.s, env.modifier_scope, s, e);
		
	}

	struct factor_modifier_group {
		factor_modifiers_parsing_state& env;
		factor_modifier_group(factor_modifiers_parsing_state& e) : env(e) {}

		void add_modifier(const triggers::trigger_and_factor& t) {
			const auto t_tag = triggers::commit_trigger(env.s.trigger_m, t.data);
			env.data.emplace_back(factor_segment{ t.factor, t_tag });
		}
	};

	struct outer_factor_modifier {
		factor_modifiers_parsing_state& env;
		

		outer_factor_modifier(factor_modifiers_parsing_state& e) : env(e) {}

		void set_factor(float f) { env.under_construction.factor = f; }
		void set_base(float f) { env.under_construction.base = f; }
		void set_days(int32_t d) { env.under_construction.factor = static_cast<float>(d); }
		void set_months(int32_t m) { env.under_construction.factor = static_cast<float>(m * 30); }
		void set_years(int32_t y) { env.under_construction.factor = static_cast<float>(y * 365); }

		void add_group(const factor_modifier_group&) {}
		void add_modifier(const triggers::trigger_and_factor& t) {
			const auto t_tag = triggers::commit_trigger(env.s.trigger_m, t.data);
			env.data.emplace_back(factor_segment{ t.factor, t_tag });
		}
	};
}

MEMBER_FDEF(modifiers::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(modifiers::crimes_preparse_file, add_crime, "crime");
MEMBER_FDEF(modifiers::nv_preparse_file, add_nv, "national_value");

MEMBER_FDEF(modifiers::outer_factor_modifier, set_factor, "factor");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_base, "base");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_days, "days");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_months, "months");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_years, "years");
MEMBER_FDEF(modifiers::outer_factor_modifier, add_group, "group");
MEMBER_FDEF(modifiers::outer_factor_modifier, add_modifier, "modifier");
MEMBER_FDEF(modifiers::factor_modifier_group, add_modifier, "modifier");



namespace modifiers {
	BEGIN_DOMAIN(crimes_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(crimes_preparse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("crime", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(national_value_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(nv_preparse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("national_value", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(modifier_factors_parsing_domain)
		BEGIN_TYPE(outer_factor_modifier)
			MEMBER_ASSOCIATION("factor", "factor", value_from_rh<float>)
			MEMBER_ASSOCIATION("base", "base", value_from_rh<float>)
			MEMBER_ASSOCIATION("months", "months", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("years", "years", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("days", "days", value_from_rh<int32_t>)
			MEMBER_TYPE_ASSOCIATION("group", "group", factor_modifier_group)
			MEMBER_TYPE_EXTERN("modifier", "modifier", triggers::trigger_and_factor, read_factor_and_trigger)
		END_TYPE
		BEGIN_TYPE(factor_modifier_group)
		    MEMBER_TYPE_EXTERN("modifier", "modifier", triggers::trigger_and_factor, read_factor_and_trigger)
		END_TYPE
	END_DOMAIN;

	void pre_parse_crimes(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		auto& main_results = state.impl->crimes_file;
		const auto fi = common_dir.open_file(u"crime.txt");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<crimes_preparse_file, crimes_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void pre_parse_national_values(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		auto& main_results = state.impl->nv_file;
		const auto fi = common_dir.open_file(u"nationalvalues.txt");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<nv_preparse_file, national_value_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	factor_tag parse_modifier_factors(
		scenario::scenario_manager& s,
		triggers::trigger_scope_state modifier_scope,
		float default_factor,
		float default_base,
		const token_group* start,
		const token_group* end) {

		factor_modifiers_parsing_state parse_state(s, modifier_scope);
		parse_state.under_construction.factor = default_factor;
		parse_state.under_construction.base = default_base;

		parse_object<outer_factor_modifier, modifier_factors_parsing_domain>(
			start,
			end,
			parse_state);
		
		auto[offset, is_new] = commit_factor(s.modifiers_m, parse_state.data);

		parse_state.under_construction.data_length = static_cast<uint16_t>(parse_state.data.size());
		parse_state.under_construction.data_offset = offset;

		if (is_new) {
			return s.modifiers_m.factor_modifiers.emplace_back(parse_state.under_construction);
		} else {
			if (auto fr = std::find(s.modifiers_m.factor_modifiers.begin(), s.modifiers_m.factor_modifiers.end(), parse_state.under_construction);
			fr != s.modifiers_m.factor_modifiers.end()) {
				return factor_tag(static_cast<value_base_of<factor_tag>>(fr - s.modifiers_m.factor_modifiers.begin()));
			} else {
				return s.modifiers_m.factor_modifiers.emplace_back(parse_state.under_construction);
			}
		}
	}
}
