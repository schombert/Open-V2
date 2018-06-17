#include "modifiers.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"
#include "triggers\\trigger_reading.h"
#include "scenario\\scenario.h"

namespace modifiers {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;

		modifiers_manager& manager;

		parsed_data crimes_file;
		parsed_data nv_file;
		parsed_data triggered_modifiers_file;

		std::vector<std::tuple<national_modifier_tag, const token_group*, const token_group*>> pending_modifier_triggers;
		std::vector<std::tuple<provincial_modifier_tag, const token_group*, const token_group*>> pending_crimes;

		parsing_environment(text_data::text_sequences& tl, modifiers_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, modifiers_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct empty_type {
		void add_unknown_key(int) {
		}
	};

	inline int inner_crime_preparse(const token_group* start, const token_group* end, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto tag = env.manager.provincial_modifiers.emplace_back();
		auto& new_i = env.manager.provincial_modifiers[tag];

		new_i.id = tag;
		new_i.name = name;

		env.manager.named_provincial_modifiers_index.emplace(name, tag);
		env.manager.crimes.emplace(tag, crime{name, triggers::trigger_tag (), tag, false});
		env.pending_crimes.emplace_back(tag, start, end);

		return 0;
	}

	struct crimes_preparse_file {
		parsing_environment& env;
		crimes_preparse_file(parsing_environment& e) : env(e) {}

		void add_crime(int) {}
	};

	struct nv_file {
		parsing_environment& env;
		nv_file(parsing_environment& e) : env(e) {}

		void add_nv(int) { }
	};

	inline int read_single_national_value(const token_group* start, const token_group* end, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		parse_national_modifier(name, env.manager, start, end);
		return 0;
	}

	inline int discard_empty_type(const token_and_type&, association_type, const empty_type&) { return 0; }
	inline token_and_type name_empty_type(const token_and_type& t, association_type, const empty_type&) { return t; }

#ifdef _DEBUG
	struct bad_modifier {};
#endif

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
			CT_STRING_INT("immigration", pack_offset_pair(provincial_offsets::immigrant_attract, bad_offset)),
			CT_STRING_INT("immigrant_attract", pack_offset_pair(provincial_offsets::immigrant_attract, bad_offset)),
			CT_STRING_INT("immigrant_push", pack_offset_pair(provincial_offsets::immigrant_push, bad_offset)),
			CT_STRING_INT("local_repair", pack_offset_pair(provincial_offsets::local_repair, bad_offset)),
			CT_STRING_INT("local_ship_build", pack_offset_pair(provincial_offsets::local_ship_build, bad_offset)),
			CT_STRING_INT("poor_savings_modifier", pack_offset_pair(bad_offset, national_offsets::poor_savings_modifier)),
			CT_STRING_INT("influence_modifier", pack_offset_pair(bad_offset, national_offsets::influence_modifier)),
			CT_STRING_INT("diplomatic_points_modifier", pack_offset_pair(bad_offset, national_offsets::diplomatic_points_modifier)),
			CT_STRING_INT("mobilisation_size", pack_offset_pair(bad_offset, national_offsets::mobilisation_size)),
			CT_STRING_INT("mobilisation_economy_impact", pack_offset_pair(bad_offset, national_offsets::mobilisation_economy_impact)),
			CT_STRING_INT("mobilization_size", pack_offset_pair(bad_offset, national_offsets::mobilisation_size)),
			CT_STRING_INT("mobilization_economy_impact", pack_offset_pair(bad_offset, national_offsets::mobilisation_economy_impact)),
			CT_STRING_INT("global_pop_militancy_modifier", pack_offset_pair(bad_offset, national_offsets::global_pop_militancy_modifier)),
			CT_STRING_INT("global_pop_consciousness_modifier", pack_offset_pair(bad_offset, national_offsets::global_pop_consciousness_modifier)),
			CT_STRING_INT("core_pop_militancy_modifier", pack_offset_pair(bad_offset, national_offsets::core_pop_militancy_modifier)),
			CT_STRING_INT("core_pop_consciousness_modifier", pack_offset_pair(bad_offset, national_offsets::core_pop_consciousness_modifier)),
			CT_STRING_INT("non_accepted_pop_militancy_modifier", pack_offset_pair(bad_offset, national_offsets::non_accepted_pop_militancy_modifier)),
			CT_STRING_INT("non_accepted_pop_consciousness_modifier", pack_offset_pair(bad_offset, national_offsets::non_accepted_pop_consciousness_modifier)),
			CT_STRING_INT("cb_generation_speed_modifier", pack_offset_pair(bad_offset, national_offsets::cb_generation_speed_modifier)),
			CT_STRING_INT("mobilization_impact", pack_offset_pair(bad_offset, national_offsets::mobilization_impact)),
			CT_STRING_INT("mobilisation_impact", pack_offset_pair(bad_offset, national_offsets::mobilization_impact)),
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
#ifdef _DEBUG
		if(value == pack_offset_pair(bad_offset, bad_offset))
			throw bad_modifier();
#endif
		return std::pair<uint32_t, uint32_t>(get_provincial_offset_from_packed(value), get_national_offset_from_packed(value));
	}

	void set_provincial_modifier(provincial_modifier_tag tag, modifier_reading_base& mod, modifiers_manager& manager) {
		auto& new_mod = manager.provincial_modifiers[tag];
		new_mod.icon = mod.icon;

		Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32> dest_vector(manager.provincial_modifier_definitions.safe_get_row(tag), provincial_offsets::aligned_32_size);
		Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32> source_vector(mod.modifier_data.data(), provincial_offsets::aligned_32_size);

		dest_vector = source_vector;

		if(mod.count_unique_national != 0) {
			mod.remove_shared_national_attributes();
			if(is_valid_index(new_mod.complement)) {
				set_national_modifier(new_mod.complement, mod, manager);
			} else {
				const auto nat_result = add_national_modifier(text_data::text_tag(), mod, manager); // creates an unindexed national modifier as complement
				new_mod.complement = nat_result;
				manager.national_modifiers[nat_result].name = new_mod.name;
			}
		}
	}

	provincial_modifier_tag add_provincial_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager) {
		const auto pmtag = manager.fetch_unique_provincial_modifier(name);
		set_provincial_modifier(pmtag, mod, manager);
		manager.provincial_modifiers[pmtag].name = name;

		return pmtag;
	}

	void set_national_modifier(national_modifier_tag tag, const modifier_reading_base& mod, modifiers_manager& manager) {
		auto& new_mod = manager.national_modifiers[tag];
		new_mod.icon = mod.icon;

		Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32> dest_vector(manager.national_modifier_definitions.safe_get_row(tag), national_offsets::aligned_32_size);
		Eigen::Map<const Eigen::VectorXf, Eigen::AlignmentType::Aligned32> source_vector(mod.modifier_data.data() + provincial_offsets::aligned_32_size, national_offsets::aligned_32_size);

		dest_vector = source_vector;
	}

	national_modifier_tag add_national_modifier(text_data::text_tag name, const modifier_reading_base& mod, modifiers_manager& manager) {
		const auto nmtag = manager.fetch_unique_national_modifier(name);
		set_national_modifier(nmtag, mod, manager);
		manager.national_modifiers[nmtag].name = name;

		return nmtag;
	}
	std::pair<provincial_modifier_tag, national_modifier_tag> add_indeterminate_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager) {
		if(mod.total_attributes == 0) {
			const auto nt = add_national_modifier(name, mod, manager);
			const auto pt = add_provincial_modifier(name, mod, manager);
			return std::pair<provincial_modifier_tag, national_modifier_tag>(pt, nt);
		} else if(mod.count_unique_provincial != 0) {
			// some attributes apply only to province
			// conclude intent is provincial modifier with additional national properties
			return std::pair<provincial_modifier_tag, national_modifier_tag>(add_provincial_modifier(name, mod, manager), national_modifier_tag());
		} else if (mod.count_unique_national == mod.total_attributes) {
			// all attributes apply only to nation
			// conclude intent is national modifier
			return std::pair<provincial_modifier_tag, national_modifier_tag>(provincial_modifier_tag(), add_national_modifier(name, mod, manager));
		} else {
			//intent indeterminate: create first a national modifier (non destructive), then destructively create provincial and complement
			const auto nt = add_national_modifier(name, mod, manager);
			const auto pt = add_provincial_modifier(name, mod, manager);

			return std::pair<provincial_modifier_tag, national_modifier_tag>(pt, nt);
		}
	}

	void modifier_reading_base::add_attribute(uint32_t provincial_offset, uint32_t national_offset, float v) {
		if(provincial_offset != bad_offset) {
			modifier_data[provincial_offset] = v;
			++count_unique_provincial;
		}
		if(national_offset != bad_offset) {
			modifier_data[provincial_offsets::aligned_32_size + national_offset] = v;
			++count_unique_national;
		}
		if((provincial_offset != bad_offset) & (national_offset != bad_offset)) {
			--count_unique_provincial;
			--count_unique_national;
		}
		if((provincial_offset != bad_offset) | (national_offset != bad_offset)) {
			++total_attributes;
		}
	}

	void modifier_reading_base::add_attribute(const std::pair<token_and_type, float>& p) {
		const auto offsets = get_provincial_and_national_offsets_from_token(p.first.start, p.first.end);
		add_attribute(offsets.first, offsets.second, p.second);
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

	inline std::pair<token_and_type, float> full_to_tf_pair(const token_and_type& t, association_type, const token_and_type& r) {
		return std::pair<token_and_type, float>(t, token_to<float>(r));
	}

	struct crime_parse_env {
		scenario::scenario_manager& s;
		crime& c;
		crime_parse_env(scenario::scenario_manager& sm, crime& cr) : s(sm), c(cr) {}
	};

	inline triggers::trigger_tag read_crime_trigger(const token_group* s, const token_group* e, crime_parse_env& env) {
		const auto td = triggers::parse_trigger(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::empty,
				false },
			s, e);
		return triggers::commit_trigger(env.s.trigger_m, td);
	}

	class crime_modifier : public modifier_reading_base {
	public:
		crime_parse_env & env;
		crime_modifier(crime_parse_env& e) : env(e) {}
		void set_trigger(triggers::trigger_tag t) {
			env.c.crime_trigger = t;
		}
		void set_active(bool v) {
			env.c.default_active = v;
		}
	};

	inline std::pair<const token_group*, const token_group*> preparse_triggered_modifier_trigger(const token_group* start, const token_group* end, parsing_environment&) {
		return std::make_pair(start, end);
	}

	class triggered_modifier : public modifier_reading_base {
	public:
		parsing_environment & env;
		std::pair<const token_group*, const token_group*> trigger = std::pair<const token_group*, const token_group*>(nullptr, nullptr);

		triggered_modifier(parsing_environment& e) : env(e) {}
		void set_trigger(const std::pair<const token_group*, const token_group*>& t) {
			trigger = t;
		}
	};

	inline std::pair<token_and_type, triggered_modifier> read_triggered_modifier(const token_and_type& t, association_type, triggered_modifier&& tm) {
		return std::pair<token_and_type, triggered_modifier>(t, std::move(tm));
	}

	class triggered_modifier_file {
	public:
		parsing_environment & env;

		triggered_modifier_file(parsing_environment& e) : env(e) {}
		void add_triggered_modifier(const std::pair<token_and_type, triggered_modifier>& p) {
			const auto tag = add_national_modifier(text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end), p.second, env.manager);
			env.pending_modifier_triggers.emplace_back(tag, p.second.trigger.first, p.second.trigger.second);
		}
	};

#define READ_PROV_MOD(name) inline int read_ ## name ## _mod(const token_group* s, const token_group* e, parsing_environment& env) { \
	static const char str[] = # name ; \
	const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, str, str + sizeof(str) - 1); \
	const auto tag = parse_provincial_modifier(name, env.manager, s, e); \
	env.manager.static_modifiers. name = tag; \
	return 0; \
}
#define READ_NAT_MOD(name) inline int read_ ## name ## _mod(const token_group* s, const token_group* e, parsing_environment& env) { \
	static const char str[] = # name ; \
	const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, str, str + sizeof(str) - 1); \
	const auto tag = parse_national_modifier(name, env.manager, s, e); \
	env.manager.static_modifiers. name = tag; \
	return 0; \
}

	inline int discard_mod(const token_group*, const token_group*, parsing_environment&) {
		return 0;
	}
	inline int read_event_mod(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		parse_indeterminate_modifier(name, env.manager, s, e);
		return 0;
	}

	READ_PROV_MOD(overseas)
	READ_PROV_MOD(coastal)
	READ_PROV_MOD(non_coastal)
	READ_PROV_MOD(coastal_sea)
	READ_PROV_MOD(sea_zone)
	READ_PROV_MOD(land_province)
	READ_PROV_MOD(blockaded)
	READ_PROV_MOD(no_adjacent_controlled)
	READ_PROV_MOD(core)
	READ_PROV_MOD(has_siege)
	READ_PROV_MOD(occupied)
	READ_PROV_MOD(nationalism)
	READ_PROV_MOD(infrastructure)
	READ_NAT_MOD(war)
	READ_NAT_MOD(peace)
	READ_NAT_MOD(disarming)
	READ_NAT_MOD(war_exhaustion)
	READ_NAT_MOD(badboy)
	READ_NAT_MOD(debt_default_to)
	READ_NAT_MOD(bad_debter)
	READ_NAT_MOD(great_power)
	READ_NAT_MOD(second_power)
	READ_NAT_MOD(civ_nation)
	READ_NAT_MOD(unciv_nation)
	READ_NAT_MOD(average_literacy)
	READ_NAT_MOD(plurality)
	READ_NAT_MOD(generalised_debt_default)
	READ_NAT_MOD(total_occupation)
	READ_NAT_MOD(total_blockaded)
	READ_NAT_MOD(in_bankrupcy)

	struct static_modifiers_file {
		parsing_environment& env;
		static_modifiers_file(parsing_environment& e) : env(e) {}

		void discard(int) {}
	};

	struct nf_environment {
		scenario::scenario_manager& s;
		text_data::text_tag group;

		nf_environment(scenario::scenario_manager& sm, text_data::text_tag g) : s(sm), group(g) {}
	};

	inline triggers::trigger_tag read_nf_limit(const token_group* s, const token_group* e, nf_environment& env) {
		const auto td = triggers::parse_trigger(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::empty,
				false},
			s, e);
		return triggers::commit_trigger(env.s.trigger_m, td);
	}

	class nf_reader : public modifier_reading_base {
	public:
		nf_environment & env;

		nf_reader(nf_environment& e) : env(e) {}

		national_focus under_construction;
		std::vector<std::pair<economy::goods_tag, float>> goods;

		void set_icon(uint8_t v) {
			under_construction.icon = v;
		}
		void set_railroads(float v) {
			under_construction.railroads = v;
		}
		void set_limit(triggers::trigger_tag t) {
			under_construction.limit = t;
		}
		void set_loyalty_value(float v) {
			under_construction.loyalty = v;
		}
		void set_ideology(const token_and_type& t) {
			under_construction.ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_outliner_show_as_percent(bool v) {
			under_construction.outliner_show_as_percent = v;
		}
		void set_flashpoint_tension(float f) {
			under_construction.flashpoint_tension = f;
		}
		void set_variable(const std::pair<token_and_type, float>& p) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			
			if(const auto ptype = tag_from_text(env.s.population_m.named_pop_type_index, name); is_valid_index(ptype)) {
				under_construction.pop_type = ptype;
				under_construction.pop_type_value = p.second;
			} else if(const auto gtype = tag_from_text(env.s.economy_m.named_goods_index, name); is_valid_index(gtype)) {
				goods.emplace_back(gtype, p.second);
			} else {
				modifier_reading_base::add_attribute(p);
			}
		}
		void discard(int) {}
	};

	inline std::pair<token_and_type, nf_reader> bind_nf(const token_and_type& l, association_type, nf_reader&& r) {
		return std::pair<token_and_type, nf_reader>(l, std::move(r));
	}

	struct nf_group_reader {
		nf_environment& env;

		nf_group_reader(nf_environment& e) : env(e) {}

		void read_nf(std::pair<token_and_type, nf_reader>&& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			const auto ptag = p.second.total_attributes != 0 ? add_provincial_modifier(name, p.second, env.s.modifiers_m) : provincial_modifier_tag();

			const auto nf_tag = env.s.modifiers_m.national_focuses.emplace_back();
			national_focus& new_nf = env.s.modifiers_m.national_focuses[nf_tag];
			new_nf = p.second.under_construction;

			new_nf.id = nf_tag;
			new_nf.name = name;
			new_nf.group = env.group;
			new_nf.modifier = ptag;

			env.s.modifiers_m.national_focus_goods_weights.resize(env.s.modifiers_m.national_focuses.size());
			for(auto gp : p.second.goods) {
				env.s.modifiers_m.national_focus_goods_weights.get(nf_tag, gp.first) = gp.second;
			}
			if(p.second.goods.size() != 0)
				new_nf.has_goods = true;
		}
	};

	void inner_read_nf_group(const token_group* s, const token_group* e, nf_environment& env);

	inline int read_nf_group(const token_group* s, const token_group* e, const token_and_type& t, scenario::scenario_manager& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.gui_m.text_data_sequences, t.start, t.end);
		nf_environment env_b(env, name);
		inner_read_nf_group(s, e, env_b);
		return 0;
	}

	struct national_focus_file {
		scenario::scenario_manager& env;
		national_focus_file(scenario::scenario_manager& e) : env(e) {}

		void read_group(int) {
			++env.modifiers_m.national_focus_group_count;
		}
	};

	struct defines_reader {
		defines& d;
		defines_reader(defines& e) : d(e) {}

		void recurse(defines_reader const&) {}
		void add_value(std::pair<token_and_type, float> const& t) {
			if(is_fixed_token_ci(t.first, "years_of_nationalism")) { d.years_of_nationalism = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_country_tax_efficiency")) { d.base_country_tax_efficiency = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_country_admin_efficiency")) { d.base_country_admin_efficiency = t.second; return; }
			if(is_fixed_token_ci(t.first, "gold_to_cash_rate")) { d.gold_to_cash_rate = t.second; return; }
			if(is_fixed_token_ci(t.first, "gold_to_worker_pay_rate")) { d.gold_to_worker_pay_rate = t.second; return; }
			if(is_fixed_token_ci(t.first, "great_nations_count")) { d.great_nations_count = t.second; return; }
			if(is_fixed_token_ci(t.first, "greatness_days")) { d.greatness_days = t.second; return; }
			if(is_fixed_token_ci(t.first, "badboy_limit")) { d.badboy_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_bureaucracy_percentage")) { d.max_bureaucracy_percentage = t.second; return; }
			if(is_fixed_token_ci(t.first, "bureaucracy_percentage_increment")) { d.bureaucracy_percentage_increment = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_crimefight_percent")) { d.min_crimefight_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_crimefight_percent")) { d.max_crimefight_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "admin_efficiency_crimefight_percent")) { d.admin_efficiency_crimefight_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "conservative_increase_after_reform")) { d.conservative_increase_after_reform = t.second; return; }
			if(is_fixed_token_ci(t.first, "campaign_event_base_time")) { d.campaign_event_base_time = t.second; return; }
			if(is_fixed_token_ci(t.first, "campaign_event_min_time")) { d.campaign_event_min_time = t.second; return; }
			if(is_fixed_token_ci(t.first, "campaign_event_state_scale")) { d.campaign_event_state_scale = t.second; return; }
			if(is_fixed_token_ci(t.first, "campaign_duration")) { d.campaign_duration = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonial_rank")) { d.colonial_rank = t.second; return; }
			if(is_fixed_token_ci(t.first, "colony_to_state_prestige_gain")) { d.colony_to_state_prestige_gain = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonial_liferating")) { d.colonial_liferating = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_greatpower_daily_influence")) { d.base_greatpower_daily_influence = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_support_reform")) { d.ai_support_reform = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_monthly_diplopoints")) { d.base_monthly_diplopoints = t.second; return; }
			if(is_fixed_token_ci(t.first, "diplomat_travel_time")) { d.diplomat_travel_time = t.second; return; }
			if(is_fixed_token_ci(t.first, "province_overseas_penalty")) { d.province_overseas_penalty = t.second; return; }
			if(is_fixed_token_ci(t.first, "noncore_tax_penalty")) { d.noncore_tax_penalty = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_tariff_efficiency")) { d.base_tariff_efficiency = t.second; return; }
			if(is_fixed_token_ci(t.first, "colony_formed_prestige")) { d.colony_formed_prestige = t.second; return; }
			if(is_fixed_token_ci(t.first, "created_cb_valid_time")) { d.created_cb_valid_time = t.second; return; }
			if(is_fixed_token_ci(t.first, "loyalty_boost_on_party_win")) { d.loyalty_boost_on_party_win = t.second; return; }
			if(is_fixed_token_ci(t.first, "movement_radicalism_base")) { d.movement_radicalism_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "movement_radicalism_passed_reform_effect")) { d.movement_radicalism_passed_reform_effect = t.second; return; }
			if(is_fixed_token_ci(t.first, "movement_radicalism_nationalism_factor")) { d.movement_radicalism_nationalism_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "suppression_points_gain_base")) { d.suppression_points_gain_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "suppress_bureaucrat_factor")) { d.suppress_bureaucrat_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "wrong_reform_militancy_impact")) { d.wrong_reform_militancy_impact = t.second; return; }
			if(is_fixed_token_ci(t.first, "suppression_radicalisation_hit")) { d.suppression_radicalisation_hit = t.second; return; }
			if(is_fixed_token_ci(t.first, "investment_score_factor")) { d.investment_score_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "unciv_tech_spread_max")) { d.unciv_tech_spread_max = t.second; return; }
			if(is_fixed_token_ci(t.first, "unciv_tech_spread_min")) { d.unciv_tech_spread_min = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_delay_between_reforms")) { d.min_delay_between_reforms = t.second; return; }
			if(is_fixed_token_ci(t.first, "economic_reform_uh_factor")) { d.economic_reform_uh_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "military_reform_uh_factor")) { d.military_reform_uh_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "wrong_reform_radical_impact")) { d.wrong_reform_radical_impact = t.second; return; }
			if(is_fixed_token_ci(t.first, "tech_year_span")) { d.tech_year_span = t.second; return; }
			if(is_fixed_token_ci(t.first, "tech_factor_vassal")) { d.tech_factor_vassal = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_suppression")) { d.max_suppression = t.second; return; }
			if(is_fixed_token_ci(t.first, "prestige_hit_on_break_country")) { d.prestige_hit_on_break_country = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_mobilize_limit")) { d.min_mobilize_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_growth_country_cache_days")) { d.pop_growth_country_cache_days = t.second; return; }
			if(is_fixed_token_ci(t.first, "newspaper_printing_frequency")) { d.newspaper_printing_frequency = t.second; return; }
			if(is_fixed_token_ci(t.first, "newspaper_timeout_period")) { d.newspaper_timeout_period = t.second; return; }
			if(is_fixed_token_ci(t.first, "newspaper_max_tension")) { d.newspaper_max_tension = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_base_supply_score_base")) { d.naval_base_supply_score_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_base_supply_score_empty")) { d.naval_base_supply_score_empty = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_base_non_core_supply_score")) { d.naval_base_non_core_supply_score = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonial_points_from_supply_factor")) { d.colonial_points_from_supply_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonial_points_for_non_core_base")) { d.colonial_points_for_non_core_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "mobilization_speed_base")) { d.mobilization_speed_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "mobilization_speed_rails_mult")) { d.mobilization_speed_rails_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_interest_lead")) { d.colonization_interest_lead = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_influence_lead")) { d.colonization_influence_lead = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_months_to_colonize")) { d.colonization_months_to_colonize = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_days_between_investment")) { d.colonization_days_between_investment = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_days_for_initial_investment")) { d.colonization_days_for_initial_investment = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_protectorate_province_maintainance")) { d.colonization_protectorate_province_maintainance = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_colony_province_maintainance")) { d.colonization_colony_province_maintainance = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_colony_industry_maintainance")) { d.colonization_colony_industry_maintainance = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_colony_railway_maintainance")) { d.colonization_colony_railway_maintainance = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_interest_cost_initial")) { d.colonization_interest_cost_initial = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_interest_cost_neighbor_modifier")) { d.colonization_interest_cost_neighbor_modifier = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_interest_cost")) { d.colonization_interest_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_influence_cost")) { d.colonization_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_extra_guard_cost")) { d.colonization_extra_guard_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_release_dominion_cost")) { d.colonization_release_dominion_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_create_state_cost")) { d.colonization_create_state_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_create_protectorate_cost")) { d.colonization_create_protectorate_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_create_colony_cost")) { d.colonization_create_colony_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_colony_state_distance")) { d.colonization_colony_state_distance = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_influence_temperature_per_day")) { d.colonization_influence_temperature_per_day = t.second; return; }
			if(is_fixed_token_ci(t.first, "colonization_influence_temperature_per_level")) { d.colonization_influence_temperature_per_level = t.second; return; }
			if(is_fixed_token_ci(t.first, "party_loyalty_hit_on_war_loss")) { d.party_loyalty_hit_on_war_loss = t.second; return; }
			if(is_fixed_token_ci(t.first, "research_points_on_conquer_mult")) { d.research_points_on_conquer_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_research_points")) { d.max_research_points = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_daily_research")) { d.max_daily_research = t.second; return; }
			if(is_fixed_token_ci(t.first, "loan_base_interest")) { d.loan_base_interest = t.second; return; }
			if(is_fixed_token_ci(t.first, "bankruptcy_external_loan_years")) { d.bankruptcy_external_loan_years = t.second; return; }
			if(is_fixed_token_ci(t.first, "bankruptcy_factor")) { d.bankruptcy_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "shadowy_financiers_max_loan_amount")) { d.shadowy_financiers_max_loan_amount = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_loan_cap_from_banks")) { d.max_loan_cap_from_banks = t.second; return; }
			if(is_fixed_token_ci(t.first, "gunboat_low_tax_cap")) { d.gunboat_low_tax_cap = t.second; return; }
			if(is_fixed_token_ci(t.first, "gunboat_high_tax_cap")) { d.gunboat_high_tax_cap = t.second; return; }
			if(is_fixed_token_ci(t.first, "gunboat_fleet_size_factor")) { d.gunboat_fleet_size_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "province_size_divider")) { d.province_size_divider = t.second; return; }
			if(is_fixed_token_ci(t.first, "capitalist_build_factory_state_employment_percent")) { d.capitalist_build_factory_state_employment_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "goods_focus_swap_chance")) { d.goods_focus_swap_chance = t.second; return; }
			if(is_fixed_token_ci(t.first, "num_closed_factories_per_state_lassiez_faire")) { d.num_closed_factories_per_state_lassiez_faire = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_num_factories_per_state_before_deleting_lassiez_faire")) { d.min_num_factories_per_state_before_deleting_lassiez_faire = t.second; return; }
			if(is_fixed_token_ci(t.first, "bankrupcy_duration")) { d.bankrupcy_duration = t.second; return; }
			if(is_fixed_token_ci(t.first, "second_rank_base_share_factor")) { d.second_rank_base_share_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "civ_base_share_factor")) { d.civ_base_share_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "unciv_base_share_factor")) { d.unciv_base_share_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "factory_paychecks_leftover_factor")) { d.factory_paychecks_leftover_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_factory_money_save")) { d.max_factory_money_save = t.second; return; }
			if(is_fixed_token_ci(t.first, "small_debt_limit")) { d.small_debt_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "factory_upgrade_employee_factor")) { d.factory_upgrade_employee_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "rgo_supply_demand_factor_hire_hi")) { d.rgo_supply_demand_factor_hire_hi = t.second; return; }
			if(is_fixed_token_ci(t.first, "rgo_supply_demand_factor_hire_lo")) { d.rgo_supply_demand_factor_hire_lo = t.second; return; }
			if(is_fixed_token_ci(t.first, "rgo_supply_demand_factor_fire")) { d.rgo_supply_demand_factor_fire = t.second; return; }
			if(is_fixed_token_ci(t.first, "employment_hire_lowest")) { d.employment_hire_lowest = t.second; return; }
			if(is_fixed_token_ci(t.first, "employment_fire_lowest")) { d.employment_fire_lowest = t.second; return; }
			if(is_fixed_token_ci(t.first, "trade_cap_low_limit_land")) { d.trade_cap_low_limit_land = t.second; return; }
			if(is_fixed_token_ci(t.first, "trade_cap_low_limit_naval")) { d.trade_cap_low_limit_naval = t.second; return; }
			if(is_fixed_token_ci(t.first, "trade_cap_low_limit_constructions")) { d.trade_cap_low_limit_constructions = t.second; return; }
			if(is_fixed_token_ci(t.first, "factory_purchase_min_factor")) { d.factory_purchase_min_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "factory_purchase_drawdown_factor")) { d.factory_purchase_drawdown_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "dig_in_increase_each_days")) { d.dig_in_increase_each_days = t.second; return; }
			if(is_fixed_token_ci(t.first, "reinforce_speed")) { d.reinforce_speed = t.second; return; }
			if(is_fixed_token_ci(t.first, "combat_difficulty_impact")) { d.combat_difficulty_impact = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_combat_width")) { d.base_combat_width = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_min_size_for_regiment")) { d.pop_min_size_for_regiment = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_size_per_regiment")) { d.pop_size_per_regiment = t.second; return; }
			if(is_fixed_token_ci(t.first, "soldier_to_pop_damage")) { d.soldier_to_pop_damage = t.second; return; }
			if(is_fixed_token_ci(t.first, "land_speed_modifier")) { d.land_speed_modifier = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_speed_modifier")) { d.naval_speed_modifier = t.second; return; }
			if(is_fixed_token_ci(t.first, "exp_gain_div")) { d.exp_gain_div = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_recruit_cost")) { d.leader_recruit_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "supply_range")) { d.supply_range = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_min_size_for_regiment_protectorate_multiplier")) { d.pop_min_size_for_regiment_protectorate_multiplier = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_min_size_for_regiment_colony_multiplier")) { d.pop_min_size_for_regiment_colony_multiplier = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_min_size_for_regiment_noncore_multiplier")) { d.pop_min_size_for_regiment_noncore_multiplier = t.second; return; }
			if(is_fixed_token_ci(t.first, "gas_attack_modifier")) { d.gas_attack_modifier = t.second; return; }
			if(is_fixed_token_ci(t.first, "combatloss_war_exhaustion")) { d.combatloss_war_exhaustion = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_max_random_prestige")) { d.leader_max_random_prestige = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_age_death_factor")) { d.leader_age_death_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_prestige_to_morale_factor")) { d.leader_prestige_to_morale_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_prestige_to_max_org_factor")) { d.leader_prestige_to_max_org_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_transfer_penalty_on_country_prestige")) { d.leader_transfer_penalty_on_country_prestige = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_prestige_land_gain")) { d.leader_prestige_land_gain = t.second; return; }
			if(is_fixed_token_ci(t.first, "leader_prestige_naval_gain")) { d.leader_prestige_naval_gain = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_seeking_chance")) { d.naval_combat_seeking_chance = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_seeking_chance_min")) { d.naval_combat_seeking_chance_min = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_self_defence_chance")) { d.naval_combat_self_defence_chance = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_shift_back_on_next_target")) { d.naval_combat_shift_back_on_next_target = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_shift_back_duration_scale")) { d.naval_combat_shift_back_duration_scale = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_speed_to_distance_factor")) { d.naval_combat_speed_to_distance_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_change_target_chance")) { d.naval_combat_change_target_chance = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_damage_org_mult")) { d.naval_combat_damage_org_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_damage_str_mult")) { d.naval_combat_damage_str_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_damage_mult_no_org")) { d.naval_combat_damage_mult_no_org = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_retreat_chance")) { d.naval_combat_retreat_chance = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_retreat_str_org_level")) { d.naval_combat_retreat_str_org_level = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_retreat_speed_mod")) { d.naval_combat_retreat_speed_mod = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_retreat_min_distance")) { d.naval_combat_retreat_min_distance = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_damaged_target_selection")) { d.naval_combat_damaged_target_selection = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_stacking_target_change")) { d.naval_combat_stacking_target_change = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_stacking_target_select")) { d.naval_combat_stacking_target_select = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_combat_max_targets")) { d.naval_combat_max_targets = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_bigship_proportion")) { d.ai_bigship_proportion = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_lightship_proportion")) { d.ai_lightship_proportion = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_transport_proportion")) { d.ai_transport_proportion = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_cavalry_proportion")) { d.ai_cavalry_proportion = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_support_proportion")) { d.ai_support_proportion = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_special_proportion")) { d.ai_special_proportion = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_escort_ratio")) { d.ai_escort_ratio = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_army_taxbase_fraction")) { d.ai_army_taxbase_fraction = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_navy_taxbase_fraction")) { d.ai_navy_taxbase_fraction = t.second; return; }
			if(is_fixed_token_ci(t.first, "ai_blockade_range")) { d.ai_blockade_range = t.second; return; }
			if(is_fixed_token_ci(t.first, "recon_unit_ratio")) { d.recon_unit_ratio = t.second; return; }
			if(is_fixed_token_ci(t.first, "engineer_unit_ratio")) { d.engineer_unit_ratio = t.second; return; }
			if(is_fixed_token_ci(t.first, "siege_brigades_min")) { d.siege_brigades_min = t.second; return; }
			if(is_fixed_token_ci(t.first, "siege_brigades_max")) { d.siege_brigades_max = t.second; return; }
			if(is_fixed_token_ci(t.first, "siege_brigades_bonus")) { d.siege_brigades_bonus = t.second; return; }
			if(is_fixed_token_ci(t.first, "recon_siege_effect")) { d.recon_siege_effect = t.second; return; }
			if(is_fixed_token_ci(t.first, "siege_attrition")) { d.siege_attrition = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_military_tactics")) { d.base_military_tactics = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_low_supply_damage_supply_status")) { d.naval_low_supply_damage_supply_status = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_low_supply_damage_days_delay")) { d.naval_low_supply_damage_days_delay = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_low_supply_damage_min_str")) { d.naval_low_supply_damage_min_str = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_low_supply_damage_per_day")) { d.naval_low_supply_damage_per_day = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_base")) { d.war_prestige_cost_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_high_prestige")) { d.war_prestige_cost_high_prestige = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_neg_prestige")) { d.war_prestige_cost_neg_prestige = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_truce")) { d.war_prestige_cost_truce = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_honor_alliance")) { d.war_prestige_cost_honor_alliance = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_honor_guarnatee")) { d.war_prestige_cost_honor_guarnatee = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_uncivilized")) { d.war_prestige_cost_uncivilized = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_prestige_cost_core")) { d.war_prestige_cost_core = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_failed_goal_militancy")) { d.war_failed_goal_militancy = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_failed_goal_prestige_base")) { d.war_failed_goal_prestige_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "war_failed_goal_prestige")) { d.war_failed_goal_prestige = t.second; return; }
			if(is_fixed_token_ci(t.first, "discredit_days")) { d.discredit_days = t.second; return; }
			if(is_fixed_token_ci(t.first, "discredit_influence_cost_factor")) { d.discredit_influence_cost_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "discredit_influence_gain_factor")) { d.discredit_influence_gain_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "banembassy_days")) { d.banembassy_days = t.second; return; }
			if(is_fixed_token_ci(t.first, "declarewar_relation_on_accept")) { d.declarewar_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "declarewar_diplomatic_cost")) { d.declarewar_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "addwargoal_relation_on_accept")) { d.addwargoal_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "addwargoal_diplomatic_cost")) { d.addwargoal_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "add_unjustified_goal_badboy")) { d.add_unjustified_goal_badboy = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_relation_on_accept")) { d.peace_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_relation_on_decline")) { d.peace_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_diplomatic_cost")) { d.peace_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "alliance_relation_on_accept")) { d.alliance_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "alliance_relation_on_decline")) { d.alliance_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "alliance_diplomatic_cost")) { d.alliance_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelalliance_relation_on_accept")) { d.cancelalliance_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelalliance_diplomatic_cost")) { d.cancelalliance_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "callally_relation_on_accept")) { d.callally_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "callally_relation_on_decline")) { d.callally_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "callally_diplomatic_cost")) { d.callally_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "askmilaccess_relation_on_accept")) { d.askmilaccess_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "askmilaccess_relation_on_decline")) { d.askmilaccess_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "askmilaccess_diplomatic_cost")) { d.askmilaccess_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelaskmilaccess_relation_on_accept")) { d.cancelaskmilaccess_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelaskmilaccess_diplomatic_cost")) { d.cancelaskmilaccess_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "givemilaccess_relation_on_accept")) { d.givemilaccess_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "givemilaccess_relation_on_decline")) { d.givemilaccess_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "givemilaccess_diplomatic_cost")) { d.givemilaccess_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelgivemilaccess_relation_on_accept")) { d.cancelgivemilaccess_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelgivemilaccess_diplomatic_cost")) { d.cancelgivemilaccess_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "warsubsidy_relation_on_accept")) { d.warsubsidy_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "warsubsidy_diplomatic_cost")) { d.warsubsidy_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelwarsubsidy_relation_on_accept")) { d.cancelwarsubsidy_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "cancelwarsubsidy_diplomatic_cost")) { d.cancelwarsubsidy_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "discredit_relation_on_accept")) { d.discredit_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "discredit_influence_cost")) { d.discredit_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "expeladvisors_relation_on_accept")) { d.expeladvisors_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "expeladvisors_influence_cost")) { d.expeladvisors_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "ceasecolonization_relation_on_accept")) { d.ceasecolonization_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "ceasecolonization_relation_on_decline")) { d.ceasecolonization_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "ceasecolonization_diplomatic_cost")) { d.ceasecolonization_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "banembassy_relation_on_accept")) { d.banembassy_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "banembassy_influence_cost")) { d.banembassy_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "increaserelation_relation_on_accept")) { d.increaserelation_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "increaserelation_relation_on_decline")) { d.increaserelation_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "increaserelation_diplomatic_cost")) { d.increaserelation_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "decreaserelation_relation_on_accept")) { d.decreaserelation_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "decreaserelation_diplomatic_cost")) { d.decreaserelation_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "addtosphere_relation_on_accept")) { d.addtosphere_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "addtosphere_influence_cost")) { d.addtosphere_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "removefromsphere_relation_on_accept")) { d.removefromsphere_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "removefromsphere_influence_cost")) { d.removefromsphere_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "removefromsphere_prestige_cost")) { d.removefromsphere_prestige_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "removefromsphere_infamy_cost")) { d.removefromsphere_infamy_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "increaseopinion_relation_on_accept")) { d.increaseopinion_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "increaseopinion_influence_cost")) { d.increaseopinion_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "decreaseopinion_relation_on_accept")) { d.decreaseopinion_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "decreaseopinion_influence_cost")) { d.decreaseopinion_influence_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "make_cb_diplomatic_cost")) { d.make_cb_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "make_cb_relation_on_accept")) { d.make_cb_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "disarmament_army_hit")) { d.disarmament_army_hit = t.second; return; }
			if(is_fixed_token_ci(t.first, "reparations_tax_hit")) { d.reparations_tax_hit = t.second; return; }
			if(is_fixed_token_ci(t.first, "prestige_reduction_base")) { d.prestige_reduction_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "prestige_reduction")) { d.prestige_reduction = t.second; return; }
			if(is_fixed_token_ci(t.first, "reparations_years")) { d.reparations_years = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_warscore_to_intervene")) { d.min_warscore_to_intervene = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_months_to_intervene")) { d.min_months_to_intervene = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_warscore_from_battles")) { d.max_warscore_from_battles = t.second; return; }
			if(is_fixed_token_ci(t.first, "gunboat_diplomatic_cost")) { d.gunboat_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "gunboat_relation_on_accept")) { d.gunboat_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "wargoal_jingoism_requirement")) { d.wargoal_jingoism_requirement = t.second; return; }
			if(is_fixed_token_ci(t.first, "liberate_state_relation_increase")) { d.liberate_state_relation_increase = t.second; return; }
			if(is_fixed_token_ci(t.first, "dishonored_callally_prestige_penalty")) { d.dishonored_callally_prestige_penalty = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_truce_months")) { d.base_truce_months = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_influence")) { d.max_influence = t.second; return; }
			if(is_fixed_token_ci(t.first, "warsubsidies_percent")) { d.warsubsidies_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "neighbour_bonus_influence_percent")) { d.neighbour_bonus_influence_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "sphere_neighbour_bonus_influence_percent")) { d.sphere_neighbour_bonus_influence_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "other_continent_bonus_influence_percent")) { d.other_continent_bonus_influence_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "puppet_bonus_influence_percent")) { d.puppet_bonus_influence_percent = t.second; return; }
			if(is_fixed_token_ci(t.first, "release_nation_prestige")) { d.release_nation_prestige = t.second; return; }
			if(is_fixed_token_ci(t.first, "release_nation_infamy")) { d.release_nation_infamy = t.second; return; }
			if(is_fixed_token_ci(t.first, "good_peace_refusal_militancy")) { d.good_peace_refusal_militancy = t.second; return; }
			if(is_fixed_token_ci(t.first, "good_peace_refusal_warexh")) { d.good_peace_refusal_warexh = t.second; return; }
			if(is_fixed_token_ci(t.first, "cb_generation_base_speed")) { d.cb_generation_base_speed = t.second; return; }
			if(is_fixed_token_ci(t.first, "cb_generation_speed_bonus_on_colony_competition")) { d.cb_generation_speed_bonus_on_colony_competition = t.second; return; }
			if(is_fixed_token_ci(t.first, "cb_generation_speed_bonus_on_colony_competition_troops_presence")) { d.cb_generation_speed_bonus_on_colony_competition_troops_presence = t.second; return; }
			if(is_fixed_token_ci(t.first, "make_cb_relation_limit")) { d.make_cb_relation_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "cb_detection_chance_base")) { d.cb_detection_chance_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "investment_influence_defense")) { d.investment_influence_defense = t.second; return; }
			if(is_fixed_token_ci(t.first, "relation_influence_modifier")) { d.relation_influence_modifier = t.second; return; }
			if(is_fixed_token_ci(t.first, "on_cb_detected_relation_change")) { d.on_cb_detected_relation_change = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_intervene_min_relations")) { d.gw_intervene_min_relations = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_intervene_max_exhaustion")) { d.gw_intervene_max_exhaustion = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_justify_cb_badboy_impact")) { d.gw_justify_cb_badboy_impact = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_cb_construction_speed")) { d.gw_cb_construction_speed = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_wargoal_jingoism_requirement_mod")) { d.gw_wargoal_jingoism_requirement_mod = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_warscore_cost_mod")) { d.gw_warscore_cost_mod = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_warscore_cost_mod_2")) { d.gw_warscore_cost_mod_2 = t.second; return; }
			if(is_fixed_token_ci(t.first, "gw_warscore_2_threshold")) { d.gw_warscore_2_threshold = t.second; return; }
			if(is_fixed_token_ci(t.first, "tension_decay")) { d.tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "tension_from_cb")) { d.tension_from_cb = t.second; return; }
			if(is_fixed_token_ci(t.first, "tension_from_movement")) { d.tension_from_movement = t.second; return; }
			if(is_fixed_token_ci(t.first, "tension_from_movement_max")) { d.tension_from_movement_max = t.second; return; }
			if(is_fixed_token_ci(t.first, "at_war_tension_decay")) { d.at_war_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "tension_on_cb_discovered")) { d.tension_on_cb_discovered = t.second; return; }
			if(is_fixed_token_ci(t.first, "tension_on_revolt")) { d.tension_on_revolt = t.second; return; }
			if(is_fixed_token_ci(t.first, "tension_while_crisis")) { d.tension_while_crisis = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_cooldown_months")) { d.crisis_cooldown_months = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_base_chance")) { d.crisis_base_chance = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_temperature_increase")) { d.crisis_temperature_increase = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_offer_diplomatic_cost")) { d.crisis_offer_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_offer_relation_on_accept")) { d.crisis_offer_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_offer_relation_on_decline")) { d.crisis_offer_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_did_not_take_side_prestige_factor_base")) { d.crisis_did_not_take_side_prestige_factor_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_did_not_take_side_prestige_factor_year")) { d.crisis_did_not_take_side_prestige_factor_year = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_winner_prestige_factor_base")) { d.crisis_winner_prestige_factor_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_winner_prestige_factor_year")) { d.crisis_winner_prestige_factor_year = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_winner_relations_impact")) { d.crisis_winner_relations_impact = t.second; return; }
			if(is_fixed_token_ci(t.first, "back_crisis_diplomatic_cost")) { d.back_crisis_diplomatic_cost = t.second; return; }
			if(is_fixed_token_ci(t.first, "back_crisis_relation_on_accept")) { d.back_crisis_relation_on_accept = t.second; return; }
			if(is_fixed_token_ci(t.first, "back_crisis_relation_on_decline")) { d.back_crisis_relation_on_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_temperature_on_offer_decline")) { d.crisis_temperature_on_offer_decline = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_temperature_participant_factor")) { d.crisis_temperature_participant_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_temperature_on_mobilize")) { d.crisis_temperature_on_mobilize = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_wargoal_infamy_mult")) { d.crisis_wargoal_infamy_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_wargoal_prestige_mult")) { d.crisis_wargoal_prestige_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_wargoal_militancy_mult")) { d.crisis_wargoal_militancy_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "crisis_interest_war_exhaustion_limit")) { d.crisis_interest_war_exhaustion_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_1_tension_decay")) { d.rank_1_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_2_tension_decay")) { d.rank_2_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_3_tension_decay")) { d.rank_3_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_4_tension_decay")) { d.rank_4_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_5_tension_decay")) { d.rank_5_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_6_tension_decay")) { d.rank_6_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_7_tension_decay")) { d.rank_7_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "rank_8_tension_decay")) { d.rank_8_tension_decay = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_fulfilled_speed")) { d.tws_fulfilled_speed = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_not_fulfilled_speed")) { d.tws_not_fulfilled_speed = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_grace_period_days")) { d.tws_grace_period_days = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_cb_limit_default")) { d.tws_cb_limit_default = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_fulfilled_idle_space")) { d.tws_fulfilled_idle_space = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_battle_min_count")) { d.tws_battle_min_count = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_battle_max_aspect")) { d.tws_battle_max_aspect = t.second; return; }
			if(is_fixed_token_ci(t.first, "large_population_influence_penalty")) { d.large_population_influence_penalty = t.second; return; }
			if(is_fixed_token_ci(t.first, "lone_backer_prestige_factor")) { d.lone_backer_prestige_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_clergy_for_literacy")) { d.base_clergy_for_literacy = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_clergy_for_literacy")) { d.max_clergy_for_literacy = t.second; return; }
			if(is_fixed_token_ci(t.first, "literacy_change_speed")) { d.literacy_change_speed = t.second; return; }
			if(is_fixed_token_ci(t.first, "assimilation_scale")) { d.assimilation_scale = t.second; return; }
			if(is_fixed_token_ci(t.first, "conversion_scale")) { d.conversion_scale = t.second; return; }
			if(is_fixed_token_ci(t.first, "immigration_scale")) { d.immigration_scale = t.second; return; }
			if(is_fixed_token_ci(t.first, "promotion_scale")) { d.promotion_scale = t.second; return; }
			if(is_fixed_token_ci(t.first, "promotion_assimilation_chance")) { d.promotion_assimilation_chance = t.second; return; }
			if(is_fixed_token_ci(t.first, "luxury_threshold")) { d.luxury_threshold = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_goods_demand")) { d.base_goods_demand = t.second; return; }
			if(is_fixed_token_ci(t.first, "base_popgrowth")) { d.base_popgrowth = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_life_rating_for_growth")) { d.min_life_rating_for_growth = t.second; return; }
			if(is_fixed_token_ci(t.first, "life_rating_growth_bonus")) { d.life_rating_growth_bonus = t.second; return; }
			if(is_fixed_token_ci(t.first, "life_need_starvation_limit")) { d.life_need_starvation_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_lack_everyday_need")) { d.mil_lack_everyday_need = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_has_everyday_need")) { d.mil_has_everyday_need = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_has_luxury_need")) { d.mil_has_luxury_need = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_no_life_need")) { d.mil_no_life_need = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_require_reform")) { d.mil_require_reform = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_ideology")) { d.mil_ideology = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_ruling_party")) { d.mil_ruling_party = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_reform_impact")) { d.mil_reform_impact = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_war_exhaustion")) { d.mil_war_exhaustion = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_non_accepted")) { d.mil_non_accepted = t.second; return; }
			if(is_fixed_token_ci(t.first, "con_literacy")) { d.con_literacy = t.second; return; }
			if(is_fixed_token_ci(t.first, "con_luxury_goods")) { d.con_luxury_goods = t.second; return; }
			if(is_fixed_token_ci(t.first, "con_poor_clergy")) { d.con_poor_clergy = t.second; return; }
			if(is_fixed_token_ci(t.first, "con_midrich_clergy")) { d.con_midrich_clergy = t.second; return; }
			if(is_fixed_token_ci(t.first, "con_reform_impact")) { d.con_reform_impact = t.second; return; }
			if(is_fixed_token_ci(t.first, "con_colonial_factor")) { d.con_colonial_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "ruling_party_happy_change")) { d.ruling_party_happy_change = t.second; return; }
			if(is_fixed_token_ci(t.first, "ruling_party_angry_change")) { d.ruling_party_angry_change = t.second; return; }
			if(is_fixed_token_ci(t.first, "pdef_base_con")) { d.pdef_base_con = t.second; return; }
			if(is_fixed_token_ci(t.first, "national_focus_divider")) { d.national_focus_divider = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_savings")) { d.pop_savings = t.second; return; }
			if(is_fixed_token_ci(t.first, "state_creation_admin_limit")) { d.state_creation_admin_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_to_join_rebel")) { d.mil_to_join_rebel = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_to_join_rising")) { d.mil_to_join_rising = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_to_autorise")) { d.mil_to_autorise = t.second; return; }
			if(is_fixed_token_ci(t.first, "reduction_after_riseing")) { d.reduction_after_riseing = t.second; return; }
			if(is_fixed_token_ci(t.first, "reduction_after_defeat")) { d.reduction_after_defeat = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_to_leadership")) { d.pop_to_leadership = t.second; return; }
			if(is_fixed_token_ci(t.first, "artisan_min_productivity")) { d.artisan_min_productivity = t.second; return; }
			if(is_fixed_token_ci(t.first, "slave_growth_divisor")) { d.slave_growth_divisor = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_hit_from_conquest")) { d.mil_hit_from_conquest = t.second; return; }
			if(is_fixed_token_ci(t.first, "luxury_con_change")) { d.luxury_con_change = t.second; return; }
			if(is_fixed_token_ci(t.first, "invention_impact_on_demand")) { d.invention_impact_on_demand = t.second; return; }
			if(is_fixed_token_ci(t.first, "artisan_suppressed_colonial_goods_category")) { d.artisan_suppressed_colonial_goods_category = t.second; return; }
			if(is_fixed_token_ci(t.first, "issue_movement_join_limit")) { d.issue_movement_join_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "issue_movement_leave_limit")) { d.issue_movement_leave_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "movement_con_factor")) { d.movement_con_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "movement_lit_factor")) { d.movement_lit_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "mil_on_reb_move")) { d.mil_on_reb_move = t.second; return; }
			if(is_fixed_token_ci(t.first, "population_suppression_factor")) { d.population_suppression_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "population_movement_radical_factor")) { d.population_movement_radical_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "nationalist_movement_mil_cap")) { d.nationalist_movement_mil_cap = t.second; return; }
			if(is_fixed_token_ci(t.first, "movement_support_uh_factor")) { d.movement_support_uh_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "rebel_occupation_strength_bonus")) { d.rebel_occupation_strength_bonus = t.second; return; }
			if(is_fixed_token_ci(t.first, "large_population_limit")) { d.large_population_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "large_population_influence_penalty_chunk")) { d.large_population_influence_penalty_chunk = t.second; return; }
			if(is_fixed_token_ci(t.first, "colony_weight")) { d.colony_weight = t.second; return; }
			if(is_fixed_token_ci(t.first, "administrator_weight")) { d.administrator_weight = t.second; return; }
			if(is_fixed_token_ci(t.first, "industryworker_weight")) { d.industryworker_weight = t.second; return; }
			if(is_fixed_token_ci(t.first, "educator_weight")) { d.educator_weight = t.second; return; }
			if(is_fixed_token_ci(t.first, "soldier_weight")) { d.soldier_weight = t.second; return; }
			if(is_fixed_token_ci(t.first, "soldier_fraction")) { d.soldier_fraction = t.second; return; }
			if(is_fixed_token_ci(t.first, "capitalist_fraction")) { d.capitalist_fraction = t.second; return; }
			if(is_fixed_token_ci(t.first, "production_weight")) { d.production_weight = t.second; return; }
			if(is_fixed_token_ci(t.first, "spam_penalty")) { d.spam_penalty = t.second; return; }
			if(is_fixed_token_ci(t.first, "one_side_max_warscore")) { d.one_side_max_warscore = t.second; return; }
			if(is_fixed_token_ci(t.first, "pop_project_investment_max_budget_factor")) { d.pop_project_investment_max_budget_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "relation_limit_no_alliance_offer")) { d.relation_limit_no_alliance_offer = t.second; return; }
			if(is_fixed_token_ci(t.first, "naval_supply_penalty_limit")) { d.naval_supply_penalty_limit = t.second; return; }
			if(is_fixed_token_ci(t.first, "chance_build_railroad")) { d.chance_build_railroad = t.second; return; }
			if(is_fixed_token_ci(t.first, "chance_build_naval_base")) { d.chance_build_naval_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "chance_build_fort")) { d.chance_build_fort = t.second; return; }
			if(is_fixed_token_ci(t.first, "chance_invest_pop_proj")) { d.chance_invest_pop_proj = t.second; return; }
			if(is_fixed_token_ci(t.first, "chance_foreign_invest")) { d.chance_foreign_invest = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_awareness_score_low_cap")) { d.tws_awareness_score_low_cap = t.second; return; }
			if(is_fixed_token_ci(t.first, "tws_awareness_score_aspect")) { d.tws_awareness_score_aspect = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_base_reluctance")) { d.peace_base_reluctance = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_time_months")) { d.peace_time_months = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_time_factor")) { d.peace_time_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_time_factor_no_goals")) { d.peace_time_factor_no_goals = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_war_exhaustion_factor")) { d.peace_war_exhaustion_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_war_direction_factor")) { d.peace_war_direction_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_war_direction_winning_mult")) { d.peace_war_direction_winning_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_force_balance_factor")) { d.peace_force_balance_factor = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_ally_base_reluctance_mult")) { d.peace_ally_base_reluctance_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_ally_time_mult")) { d.peace_ally_time_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_ally_war_exhaustion_mult")) { d.peace_ally_war_exhaustion_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_ally_war_direction_mult")) { d.peace_ally_war_direction_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "peace_ally_force_balance_mult")) { d.peace_ally_force_balance_mult = t.second; return; }
			if(is_fixed_token_ci(t.first, "aggression_base")) { d.aggression_base = t.second; return; }
			if(is_fixed_token_ci(t.first, "aggression_unciv_bonus")) { d.aggression_unciv_bonus = t.second; return; }
			if(is_fixed_token_ci(t.first, "fleet_size")) { d.fleet_size = t.second; return; }
			if(is_fixed_token_ci(t.first, "min_fleets")) { d.min_fleets = t.second; return; }
			if(is_fixed_token_ci(t.first, "max_fleets")) { d.max_fleets = t.second; return; }
			if(is_fixed_token_ci(t.first, "months_before_disband")) { d.months_before_disband = t.second; return; }
		}
		void start_date(date_tag t) {
			d.start_date = t;
		}
		void end_date(date_tag t) {
			d.end_date = t;
		}
	};

	inline defines_reader const& dr_from_full(token_and_type const&, association_type, defines_reader const& r) { return r; }
}

MEMBER_FDEF(modifiers::defines_reader, start_date, "start_date");
MEMBER_FDEF(modifiers::defines_reader, end_date, "end_date");
MEMBER_FDEF(modifiers::defines_reader, recurse, "recurse");
MEMBER_FDEF(modifiers::defines_reader, add_value, "value");
MEMBER_FDEF(modifiers::nf_reader, set_icon, "icon");
MEMBER_FDEF(modifiers::nf_reader, set_railroads, "railroads");
MEMBER_FDEF(modifiers::nf_reader, set_limit, "limit");
MEMBER_FDEF(modifiers::nf_reader, set_loyalty_value, "loyalty_value");
MEMBER_FDEF(modifiers::nf_reader, set_ideology, "ideology");
MEMBER_FDEF(modifiers::nf_reader, set_outliner_show_as_percent, "outliner_show_as_percent");
MEMBER_FDEF(modifiers::nf_reader, set_flashpoint_tension, "flashpoint_tension");
MEMBER_FDEF(modifiers::nf_reader, set_variable, "variable");
MEMBER_FDEF(modifiers::nf_reader, discard, "discard");
MEMBER_FDEF(modifiers::nf_group_reader, read_nf, "nf");
MEMBER_FDEF(modifiers::national_focus_file, read_group, "group");
MEMBER_FDEF(modifiers::static_modifiers_file, discard, "modifier");
MEMBER_FDEF(modifiers::triggered_modifier_file, add_triggered_modifier, "modifier");
MEMBER_FDEF(modifiers::triggered_modifier, set_trigger, "trigger");
MEMBER_DEF(modifiers::triggered_modifier, icon, "icon");
MEMBER_FDEF(modifiers::triggered_modifier, add_attribute, "attribute");
MEMBER_FDEF(modifiers::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(modifiers::crimes_preparse_file, add_crime, "crime");
MEMBER_FDEF(modifiers::nv_file, add_nv, "national_value");
MEMBER_DEF(modifiers::modifier_reading_base, icon, "icon");
MEMBER_FDEF(modifiers::modifier_reading_base, add_attribute, "attribute");
MEMBER_FDEF(modifiers::crime_modifier, set_trigger, "trigger");
MEMBER_FDEF(modifiers::crime_modifier, set_active, "active");
MEMBER_DEF(modifiers::crime_modifier, icon, "icon");
MEMBER_FDEF(modifiers::crime_modifier, add_attribute, "attribute");

MEMBER_FDEF(modifiers::outer_factor_modifier, set_factor, "factor");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_base, "base");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_days, "days");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_months, "months");
MEMBER_FDEF(modifiers::outer_factor_modifier, set_years, "years");
MEMBER_FDEF(modifiers::outer_factor_modifier, add_group, "group");
MEMBER_FDEF(modifiers::outer_factor_modifier, add_modifier, "modifier");
MEMBER_FDEF(modifiers::factor_modifier_group, add_modifier, "modifier");


namespace modifiers {
	BEGIN_DOMAIN(defines_domain)
		BEGIN_TYPE(defines_reader)
		MEMBER_ASSOCIATION("start_date", "start_date", value_from_rh<date_tag>)
		MEMBER_ASSOCIATION("end_date", "end_date", value_from_rh<date_tag>)
		MEMBER_VARIABLE_ASSOCIATION("value", accept_all, full_to_tf_pair)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("recurse", accept_all, defines_reader, dr_from_full)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(nf_group_domain)
		BEGIN_TYPE(nf_group_reader)
			MEMBER_VARIABLE_TYPE_ASSOCIATION("nf", accept_all, nf_reader, bind_nf)
		END_TYPE
		BEGIN_TYPE(nf_reader)
			MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint8_t>)
			MEMBER_ASSOCIATION("railroads", "railroads", value_from_rh<float>)
			MEMBER_ASSOCIATION("loyalty_value", "loyalty_value", value_from_rh<float>)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("outliner_show_as_percent", "outliner_show_as_percent", value_from_rh<bool>)
			MEMBER_ASSOCIATION("flashpoint_tension", "flashpoint_tension", value_from_rh<float>)
			MEMBER_TYPE_EXTERN("limit", "limit", triggers::trigger_tag, read_nf_limit)
			MEMBER_VARIABLE_ASSOCIATION("variable", accept_all, full_to_tf_pair)
			MEMBER_ASSOCIATION("discard", "only_great_power", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "uncolonized_province", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "increase_colony", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "colonial_validity_check", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "own_provinces", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "has_flashpoint", discard_from_rh)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(nf_file_domain)
		BEGIN_TYPE(national_focus_file)
			MEMBER_VARIABLE_TYPE_EXTERN("group", accept_all, int, read_nf_group)
		END_TYPE
	END_DOMAIN

	BEGIN_DOMAIN(event_modifiers_domain)
		BEGIN_TYPE(static_modifiers_file)
			MEMBER_VARIABLE_TYPE_EXTERN("modifier", accept_all, int, read_event_mod)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(static_modifier_domain)
		BEGIN_TYPE(static_modifiers_file)
			MEMBER_TYPE_EXTERN("modifier", "very_easy_player", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "easy_player", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "hard_player", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "very_hard_player", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "very_easy_ai", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "easy_ai", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "hard_ai", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "very_hard_ai", int, discard_mod)
			MEMBER_TYPE_EXTERN("modifier", "overseas", int, read_overseas_mod)
			MEMBER_TYPE_EXTERN("modifier", "coastal", int, read_coastal_mod)
			MEMBER_TYPE_EXTERN("modifier", "non_coastal", int, read_non_coastal_mod)
			MEMBER_TYPE_EXTERN("modifier", "coastal_sea", int, read_coastal_sea_mod)
			MEMBER_TYPE_EXTERN("modifier", "sea_zone", int, read_sea_zone_mod)
			MEMBER_TYPE_EXTERN("modifier", "land_province", int, read_land_province_mod)
			MEMBER_TYPE_EXTERN("modifier", "blockaded", int, read_blockaded_mod)
			MEMBER_TYPE_EXTERN("modifier", "no_adjacent_controlled", int, read_no_adjacent_controlled_mod)
			MEMBER_TYPE_EXTERN("modifier", "core", int, read_core_mod)
			MEMBER_TYPE_EXTERN("modifier", "has_siege", int, read_has_siege_mod)
			MEMBER_TYPE_EXTERN("modifier", "occupied", int, read_occupied_mod)
			MEMBER_TYPE_EXTERN("modifier", "nationalism", int, read_nationalism_mod)
			MEMBER_TYPE_EXTERN("modifier", "infrastructure", int, read_infrastructure_mod)
			MEMBER_TYPE_EXTERN("modifier", "war", int, read_war_mod)
			MEMBER_TYPE_EXTERN("modifier", "peace", int, read_peace_mod)
			MEMBER_TYPE_EXTERN("modifier", "disarming", int, read_disarming_mod)
			MEMBER_TYPE_EXTERN("modifier", "war_exhaustion", int, read_war_exhaustion_mod)
			MEMBER_TYPE_EXTERN("modifier", "badboy", int, read_badboy_mod)
			MEMBER_TYPE_EXTERN("modifier", "debt_default_to", int, read_debt_default_to_mod)
			MEMBER_TYPE_EXTERN("modifier", "bad_debter", int, read_bad_debter_mod)
			MEMBER_TYPE_EXTERN("modifier", "great_power", int, read_great_power_mod)
			MEMBER_TYPE_EXTERN("modifier", "second_power", int, read_second_power_mod)
			MEMBER_TYPE_EXTERN("modifier", "civ_nation", int, read_civ_nation_mod)
			MEMBER_TYPE_EXTERN("modifier", "unciv_nation", int, read_unciv_nation_mod)
			MEMBER_TYPE_EXTERN("modifier", "average_literacy", int, read_average_literacy_mod)
			MEMBER_TYPE_EXTERN("modifier", "plurality", int, read_plurality_mod)
			MEMBER_TYPE_EXTERN("modifier", "generalised_debt_default", int, read_generalised_debt_default_mod)
			MEMBER_TYPE_EXTERN("modifier", "total_occupation", int, read_total_occupation_mod)
			MEMBER_TYPE_EXTERN("modifier", "total_blockaded", int, read_total_blockaded_mod)
			MEMBER_TYPE_EXTERN("modifier", "in_bankrupcy", int, read_in_bankrupcy_mod)
			MEMBER_TYPE_EXTERN("modifier", "base_values", int, discard_mod)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(single_modifier_domain)
		BEGIN_TYPE(modifier_reading_base)
			MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint32_t>)
			MEMBER_VARIABLE_ASSOCIATION("attribute", accept_all, full_to_tf_pair)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(pre_parse_triggered_modifier_domain)
		BEGIN_TYPE(triggered_modifier_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("modifier", accept_all, triggered_modifier, read_triggered_modifier)
		END_TYPE
		BEGIN_TYPE(triggered_modifier)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint32_t>)
		MEMBER_TYPE_EXTERN("trigger", "trigger", triggers::trigger_tag, preparse_triggered_modifier_trigger)
		MEMBER_VARIABLE_ASSOCIATION("attribute", accept_all, full_to_tf_pair)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(crime_modifier_domain)
		BEGIN_TYPE(crime_modifier)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("active", "active", value_from_rh<bool>)
		MEMBER_TYPE_EXTERN("trigger", "trigger", triggers::trigger_tag, read_crime_trigger)
		MEMBER_VARIABLE_ASSOCIATION("attribute", accept_all, full_to_tf_pair)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(crimes_pre_parsing_domain)
		BEGIN_TYPE(crimes_preparse_file)
		MEMBER_VARIABLE_TYPE_EXTERN("crime", accept_all, int, inner_crime_preparse)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(national_value_parsing_domain)
		BEGIN_TYPE(nv_file)
		MEMBER_VARIABLE_TYPE_EXTERN("national_value", accept_all, int, read_single_national_value)
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

	void inner_read_nf_group(const token_group* s, const token_group* e, nf_environment& env) {
		parse_object<nf_group_reader, nf_group_domain>(s, e, env);
	}

	void read_defines(modifiers_manager& m, const directory& source_directory) {
		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto fi = common_dir.open_file(u"defines.lua");

		if(fi) {
			const auto sz = fi->size();
			auto const parse_data = std::unique_ptr<char[]>(new char[sz]);
			fi->read_to_buffer(parse_data.get(), sz);

			std::vector<token_group> parse_results;
			parse_lua_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(parse_results.size() != 0) {
				parse_object<defines_reader, defines_domain>(parse_results.data(), parse_results.data() + parse_results.size(), m.global_defines);
			}
		}
	}

	void read_national_focuses(scenario::scenario_manager& s, const directory& source_directory) {
		const auto common_dir = source_directory.get_directory(u"\\common");
		parsed_data main_results;
		const auto fi = common_dir.open_file(u"national_focus.txt");

		s.modifiers_m.national_focus_goods_weights.reset(static_cast<uint32_t>(s.economy_m.goods.size()));

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<national_focus_file, nf_file_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					s);
			}
		}
	}

	void read_static_modifiers(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		parsed_data main_results;
		const auto fi = common_dir.open_file(u"static_modifiers.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<static_modifiers_file, static_modifier_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void read_event_modifiers(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		parsed_data main_results;
		const auto fi = common_dir.open_file(u"event_modifiers.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<static_modifiers_file, event_modifiers_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	provincial_modifier_tag parse_provincial_modifier(
		text_data::text_tag name,
		modifiers_manager& manager,
		const token_group* s,
		const token_group* e) {

		auto parsed_modifier = parse_object<modifier_reading_base, single_modifier_domain>(s, e);
		return add_provincial_modifier(name, parsed_modifier, manager);
	}

	national_modifier_tag parse_national_modifier(
		text_data::text_tag name,
		modifiers_manager& manager,
		const token_group* s,
		const token_group* e) {

		auto parsed_modifier = parse_object<modifier_reading_base, single_modifier_domain>(s, e);
		return add_national_modifier(name, parsed_modifier, manager);
	}

	std::pair<provincial_modifier_tag, national_modifier_tag> parse_indeterminate_modifier(
		text_data::text_tag name,
		modifiers_manager& manager,
		const token_group* s,
		const token_group* e) {

		auto parsed_modifier = parse_object<modifier_reading_base, single_modifier_domain>(s, e);
		return add_indeterminate_modifier(name, parsed_modifier, manager);
	}

	void pre_parse_triggered_modifiers(parsing_state& state, const directory& source_directory) {
		const auto common_dir = source_directory.get_directory(u"\\common");
		auto& main_results = state.impl->triggered_modifiers_file;
		const auto fi = common_dir.open_file(u"triggered_modifiers.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<triggered_modifier_file, pre_parse_triggered_modifier_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

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
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void read_crimes(parsing_state& state, scenario::scenario_manager& s) {
		for(const auto& t : state.impl->pending_crimes) {
			crime& this_crime = s.modifiers_m.crimes[std::get<0>(t)];
			crime_parse_env env(s, this_crime);
			auto res = parse_object<crime_modifier, crime_modifier_domain>(std::get<1>(t), std::get<2>(t), env);
			set_provincial_modifier(std::get<0>(t), res, s.modifiers_m);
		}
	}

	void read_national_values(
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
				parse_object<nv_file, national_value_parsing_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
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

	void read_triggered_modifiers(parsing_state& state, scenario::scenario_manager& s) {
		for(const auto& t : state.impl->pending_modifier_triggers) {
			if(std::get<1>(t) == std::get<2>(t)) {
				s.modifiers_m.triggered_modifiers.emplace_back(std::get<0>(t), triggers::trigger_tag());
			} else {
				const auto td = triggers::parse_trigger(s,
					triggers::trigger_scope_state{
						triggers::trigger_slot_contents::nation,
						triggers::trigger_slot_contents::nation,
						triggers::trigger_slot_contents::empty,
						false},
					std::get<1>(t), std::get<2>(t));
				s.modifiers_m.triggered_modifiers.emplace_back(std::get<0>(t), triggers::commit_trigger(s.trigger_m, td));
			}
		}
	}
}
