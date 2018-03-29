#include "modifiers.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"

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
			CT_STRING_INT("min_build_fort", pack_offset_pair(provincial_offsets::min_build_fort, bad_offset))
		>;
		using token_tree = typename sorted<token_tree_unsorted>::type;
	}
}

MEMBER_FDEF(modifiers::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(modifiers::crimes_preparse_file, add_crime, "crime");
MEMBER_FDEF(modifiers::nv_preparse_file, add_nv, "national_value");

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
}
