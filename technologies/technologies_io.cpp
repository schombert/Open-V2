#include "common\\common.h"
#include "technologies_io.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "modifiers\\modifiers_io.h"
#include "simple_fs\\simple_fs.h"
#include "scenario\\scenario.h"
#include "triggers\\trigger_reading.h"

namespace technologies {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;
		const directory tech_directory;
		technologies_manager& manager;
		modifiers::modifiers_manager& mod_manager;

		parsed_data main_file_parse_tree;
		std::vector<std::pair<tech_category_tag, parsed_data>> tech_files_parse_trees;
		std::vector<parsed_data> inventions_parse_trees;
		std::vector<std::tuple<tech_tag, const token_group*, const token_group*>> pending_technologies;
		std::vector<std::tuple<tech_tag, const token_group*, const token_group*>> pending_inventions;

		parsing_environment(text_data::text_sequences& tl, const directory& tech_root, technologies_manager& m, modifiers::modifiers_manager& mm) :
			text_lookup(tl), tech_directory(tech_root), manager(m), mod_manager(mm) {}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, const directory& tech_directory, technologies_manager& m, modifiers::modifiers_manager& mm) :
		impl(std::make_unique<parsing_environment>(tl, tech_directory, m, mm)) {}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct folder {
		parsing_environment& env;

		std::vector<text_data::text_tag> subcategories;

		folder(parsing_environment& e) : env(e) {}

		void add_sub_category(const token_and_type& t) {
			subcategories.push_back(text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end));
		}

		void commit_sub_categories(tech_category_tag folder_tag) const {
			int32_t i = 0;
			auto& parent = env.manager.technology_categories[folder_tag];

			for(auto tt : subcategories) {
				const auto new_tag = env.manager.technology_subcategories.emplace_back();
				auto& new_subcat = env.manager.technology_subcategories[new_tag];

				new_subcat.id = new_tag;
				new_subcat.name = tt;
				new_subcat.parent = folder_tag;

				env.manager.named_subcategory_index.emplace(tt, new_tag);

				if(static_cast<size_t>(i) < std::extent_v<decltype(parent.subcategories)>)
					parent.subcategories[i] = new_tag;
				++i;
			}
		}
	};

	struct folders {
		parsing_environment& env;

		folders(parsing_environment& e) : env(e) {}

		void add_folder(const std::pair<token_and_type, folder>& fp) {
			const auto category_name = text_data::get_thread_safe_text_handle(env.text_lookup, fp.first.start, fp.first.end);
			const auto new_category_tag = env.manager.technology_categories.emplace_back();
			auto& new_category = env.manager.technology_categories[new_category_tag];

			new_category.id = new_category_tag;
			new_category.name = category_name;

			env.manager.named_category_index.emplace(category_name, new_category_tag);

			fp.second.commit_sub_categories(new_category_tag);

			env.tech_files_parse_trees.emplace_back();
			auto& file_pair = env.tech_files_parse_trees.back();
			file_pair.first = new_category_tag;
			auto& result = file_pair.second;

			//read folder file

			std::u16string folder_file(fp.first.start, fp.first.end);
			folder_file += u".txt";

			const auto tech_file = env.tech_directory.open_file(folder_file);

			if(tech_file) {
				const auto sz = tech_file->size();
				result.parse_data = std::unique_ptr<char[]>(new char[sz]);

				tech_file->read_to_buffer(result.parse_data.get(), sz);
				parse_pdx_file(file_pair.second.parse_results, result.parse_data.get(), result.parse_data.get() + sz);

				if(result.parse_results.size() != 0)
					pre_parse_single_tech_file(env, result.parse_results.data(), result.parse_results.data() + result.parse_results.size());
			}
		}
	};

	inline std::pair<token_and_type, folder> bind_folder(const token_and_type& t, association_type, folder&& f) {
		return std::pair<token_and_type, folder>(t, std::move(f));
	}

	struct parse_schools {
		parsing_environment& env;

		parse_schools(parsing_environment& e) : env(e) {}

		void add_school(const std::pair<text_data::text_tag, modifiers::national_modifier_tag>&) {
		}
	};

	inline std::pair<text_data::text_tag, modifiers::national_modifier_tag> read_school_modifier(const token_group* start, const token_group* end, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		return std::make_pair(name, modifiers::parse_national_modifier(name, env.mod_manager, start, end));
	}

	struct technologies_file {
		parsing_environment& env;

		technologies_file(parsing_environment& e) : env(e) {}
		void handle_folders(const folders&) {}
		void handle_schools(const parse_schools&) {}

		void add_unknown_key(int) {}
	};

	inline int inner_pre_parse_tech(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env) {
		const auto tech_name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto tag = env.manager.technologies_container.emplace_back();
		auto& new_tech = env.manager.technologies_container[tag];

		new_tech.id = tag;
		new_tech.name = tech_name;

		env.manager.named_technology_index.emplace(tech_name, tag);

		env.pending_technologies.emplace_back(tag, s, e);

		return 0;
	}

	struct specific_tech_file {
		parsing_environment& env;

		specific_tech_file(parsing_environment& e) : env(e) {}

		void insert_tech(int) {}

		void add_unknown_key(int) {}
	};

	inline int inner_pre_parse_invention(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto tag = env.manager.technologies_container.emplace_back();
		auto& new_tech = env.manager.technologies_container[tag];

		new_tech.id = tag;
		new_tech.name = name;
		env.manager.named_technology_index.emplace(name, tag);
		env.manager.inventions.push_back(tag);

		env.pending_inventions.emplace_back(tag, s, e);

		return 0;
	}

	struct inventions_pre_parse_file {
		parsing_environment& env;

		inventions_pre_parse_file(parsing_environment& e) : env(e) {}

		void add_invention(int) {}
	};

#ifdef _DEBUG
	struct bad_factory {};
	struct bad_crime {};
	struct bad_unit {};
	struct bad_good {};
	struct bad_area {};
	struct bad_rebel_type {};
	struct too_many_techs {};
#endif
	struct tech_reading_env {
		scenario::scenario_manager& s;
		technology& under_construction;
		modifiers::modifier_reading_base mod;

		tech_reading_env(scenario::scenario_manager& sm, technology& t) : s(sm), under_construction(t) {}
	};

	inline modifiers::factor_tag read_tech_chance(const token_group* s, const token_group* e, tech_reading_env& env) {
		return modifiers::parse_modifier_factors(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::empty,
				false },
				1.0f, 0.0f, s, e);
	}

	inline triggers::trigger_tag read_tech_allow(const token_group* s, const token_group* e, tech_reading_env& env) {
		const auto td = triggers::parse_trigger(env.s, triggers::trigger_scope_state{
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::empty,
			false }, s, e);
		return triggers::commit_trigger(env.s.trigger_m, td);
	}
#define MATTRIB(name ) void set_ ## name (military::unit_attribute_type v) { attributes[military::unit_attribute:: name] = v;}

	struct tech_subunit {
		tech_reading_env & env;
		military::unit_attribute_vector attributes;

		tech_subunit(tech_reading_env& e) : env(e) {}
		MATTRIB(hull)
			MATTRIB(attack)
			MATTRIB(fire_range)
			MATTRIB(support)
			MATTRIB(evasion)
			MATTRIB(speed)
			MATTRIB(organization)
			MATTRIB(build_time)
			MATTRIB(supply_consumption)
			MATTRIB(strength)
			MATTRIB(siege)
			MATTRIB(discipline)
	};

#undef MATTRIB

	inline std::pair<token_and_type, float> bind_token_float(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, float>(l, token_to<float>(r));
	}

	struct tech_goods_list {
		tech_reading_env & env;
		std::vector<std::pair<economy::goods_tag, float>> goods;

		tech_goods_list(tech_reading_env& e) : env(e) {}

		void add_good(const std::pair<token_and_type, float>& p) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			const auto gt = tag_from_text(env.s.economy_m.named_goods_index, name);
			goods.emplace_back(gt, p.second);
#ifdef _DEBUG
			if(!is_valid_index(gt)) {
				throw bad_good();
			}
#endif
		}
	};

	struct rebel_org_gain {
		tech_reading_env & env;
		population::rebel_type_tag faction;
		float value = 0.0f;

		rebel_org_gain(tech_reading_env& e) : env(e) {}

		void set_faction(const token_and_type& t) {
			if(is_fixed_token_ci(t, "all")) {

			} else {
				const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
				const auto rt = tag_from_text(env.s.population_m.named_rebel_type_index, name);
				faction = rt;
#ifdef _DEBUG
				if(!is_valid_index(rt)) {
					throw bad_rebel_type();
				}
#endif
			}
		}
	};

	inline std::pair<token_and_type, tech_subunit> bind_subunit(const token_and_type& l, association_type, tech_subunit&& u) {
		return std::pair<token_and_type, tech_subunit>(l, std::move(u));
	}

	struct tech_reader;

	struct invention_reader {
		tech_reading_env & env;
		invention_reader(tech_reading_env& e) : env(e) {}

		void set_effect(const tech_reader&) {}
		void set_chance(modifiers::factor_tag t) {
			env.under_construction.ai_chance = t;
		}
		void set_limit(triggers::trigger_tag t) {
			env.under_construction.allow = t;
		}
		void discard(int) {}
		void pass_modifier(const std::pair<token_and_type, float>& p) {
			env.mod.add_attribute(p);
		}
	};

	struct tech_reader {
		tech_reading_env & env;

		tech_reader(tech_reading_env& e) : env(e) {}

		void add_subunit(const std::pair<token_and_type, tech_subunit>& p) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			const auto ut = tag_from_text(env.s.military_m.named_unit_type_index, name);
#ifdef _DEBUG
			if(!is_valid_index(ut)) {
				throw bad_unit();
			}
#endif
			if(!is_valid_index(env.under_construction.unit_adjustment)) {
				env.under_construction.unit_adjustment = 
					unit_adjustment_tag(static_cast<unit_adjustment_tag::value_base_t>(env.s.technology_m.unit_type_adjustments.outer_size()));
				env.s.technology_m.unit_type_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}
			env.s.technology_m.unit_type_adjustments.get(env.under_construction.unit_adjustment, ut) = p.second.attributes;
		}
		void set_year(uint16_t v) {
			env.under_construction.year = v;
		}
		void set_cost(uint16_t v) {
			env.under_construction.cost = v;
		}
		void set_ai_chance(modifiers::factor_tag t) {
			env.under_construction.ai_chance = t;
		}
		void set_activate_building(const token_and_type& t) {
			if(is_fixed_token_ci(t, "naval_base")) {
				env.under_construction.flags |= technology::activate_naval_base;
			} else if(is_fixed_token_ci(t, "fort")) {
				env.under_construction.flags |= technology::activate_fort;
			} else if(is_fixed_token_ci(t, "railroad")) {
				env.under_construction.flags |= technology::activate_railroad;
			} else {
				const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
				const auto factory = tag_from_text(env.s.economy_m.named_factory_types_index, name);
#ifdef _DEBUG
				if(!is_valid_index(factory)) {
					throw bad_factory();
				}
#endif
				env.under_construction.activate_factory = factory;
			}
		}
		void set_enable_crime(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
			const auto crime = tag_from_text(env.s.modifiers_m.named_provincial_modifiers_index, name);
			env.under_construction.enable_crime = crime;
#ifdef _DEBUG
			if(!is_valid_index(crime)) {
				throw bad_crime();
			}
#endif
		}
		void set_allow(triggers::trigger_tag t) {
			env.under_construction.allow = t;
		}
		void set_area(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
			const auto sub_cat_tag = tag_from_text(env.s.technology_m.named_subcategory_index, name);

			if(is_valid_index(sub_cat_tag)) {
				auto& parent_category = env.s.technology_m.technology_subcategories[sub_cat_tag];
				for(int32_t i = 0; i < static_cast<int32_t>(std::extent_v<decltype(parent_category.member_techs)>); ++i) {
					if(!is_valid_index(parent_category.member_techs[i])) {
						parent_category.member_techs[i] = env.under_construction.id;
						return;
					}
				}
#ifdef _DEBUG
				throw too_many_techs();
#endif
			} else {
#ifdef _DEBUG
				throw bad_area();
#endif
			}

		}
		void discard(int) {}
		void activate_unit(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
			const auto ut = tag_from_text(env.s.military_m.named_unit_type_index, name);

			if(is_valid_index(ut)) {
				if(!is_valid_index(env.under_construction.unit_adjustment)) {
					env.under_construction.unit_adjustment =
						unit_adjustment_tag(static_cast<unit_adjustment_tag::value_base_t>(env.s.technology_m.unit_type_adjustments.outer_size()));
					env.s.technology_m.unit_type_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
				}
				env.s.technology_m.unit_type_adjustments.get(env.under_construction.unit_adjustment, ut)[military::unit_attribute::enabled] = military::unit_attribute_type(1);
			} else {
#ifdef _DEBUG
				throw bad_unit();
#endif
			}
		}
		void deactivate_unit(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
			const auto ut = tag_from_text(env.s.military_m.named_unit_type_index, name);

			if(is_valid_index(ut)) {
				if(!is_valid_index(env.under_construction.unit_adjustment)) {
					env.under_construction.unit_adjustment =
						unit_adjustment_tag(static_cast<unit_adjustment_tag::value_base_t>(env.s.technology_m.unit_type_adjustments.outer_size()));
					env.s.technology_m.unit_type_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
				}
				env.s.technology_m.unit_type_adjustments.get(env.under_construction.unit_adjustment, ut)[military::unit_attribute::enabled] = military::unit_attribute_type(-1);
			} else {
#ifdef _DEBUG
				throw bad_unit();
#endif
			}
		}
		void set_unciv_military(bool v) {
			if(v)
				env.under_construction.flags |= technology::unciv_military;
		}
		void set_shared_prestige(float v) {
			env.under_construction.shared_prestige = v;
		}
		void set_pop_growth(float v) {
			env.mod.add_attribute(modifiers::bad_offset, modifiers::national_offsets::global_population_growth, v * 10.0f);
		}
		void set_increase_research(float v) {
			env.mod.add_attribute(modifiers::bad_offset, modifiers::national_offsets::research_points_modifier, v);
		}
		void set_administrative_efficiency(float v) {
			env.mod.add_attribute(modifiers::bad_offset, modifiers::national_offsets::administrative_efficiency_modifier, v);
		}
		void set_diplomatic_points(float v) {
			env.mod.add_attribute(modifiers::bad_offset, modifiers::national_offsets::diplomatic_points_modifier, v);
		}
		void set_tax_eff(float v) {
			env.mod.add_attribute(modifiers::bad_offset, modifiers::national_offsets::tax_efficiency, v);
		}
		void set_rebel_org_gain(const rebel_org_gain& v) {
			if(is_valid_index(v.faction)) {
				if(!is_valid_index(env.under_construction.rebel_adjustment)) {
					env.under_construction.rebel_adjustment =
						rebel_adjustment_tag(static_cast<rebel_adjustment_tag::value_base_t>(env.s.technology_m.rebel_org_gain.outer_size()));
					env.s.technology_m.rebel_org_gain.resize(to_index(env.under_construction.unit_adjustment) + 1);
				}
				env.s.technology_m.rebel_org_gain.get(env.under_construction.rebel_adjustment, v.faction) += v.value;
			} else {
				if(!is_valid_index(env.under_construction.rebel_adjustment)) {
					env.under_construction.rebel_adjustment =
						rebel_adjustment_tag(static_cast<rebel_adjustment_tag::value_base_t>(env.s.technology_m.rebel_org_gain.outer_size()));
					env.s.technology_m.rebel_org_gain.resize(to_index(env.under_construction.unit_adjustment) + 1);
				}

				const auto max = static_cast<population::rebel_type_tag::value_base_t>(env.s.population_m.rebel_types.size());
				for(population::rebel_type_tag::value_base_t i = 0; i < max; ++i) {
					env.s.technology_m.rebel_org_gain.get(env.under_construction.rebel_adjustment, population::rebel_type_tag(i)) += v.value;
				}
			}
		}
		void set_max_national_focus(float v) {
			env.under_construction.attributes[tech_offset::max_national_focus] = v;
		}
		void set_rgo_size(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::rgo_size>(p.first)) += p.second;
		}
		void set_factory_goods_input(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::factory_goods_input>(p.first)) += p.second;
		}
		void set_factory_goods_output(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::factory_goods_output>(p.first)) += p.second;
		}
		void set_factory_goods_throughput(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::factory_goods_throughput>(p.first)) += p.second;
		}
		void set_rgo_goods_output(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::rgo_goods_output>(p.first)) += p.second;
		}
		void set_rgo_goods_throughput(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::rgo_goods_throughput>(p.first)) += p.second;
		}
		void set_artisan_goods_input(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::artisan_goods_input>(p.first)) += p.second;
		}
		void set_artisan_goods_output(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::artisan_goods_output>(p.first)) += p.second;
		}
		void set_artisan_goods_throughput(const tech_goods_list& v) {
			if(!is_valid_index(env.under_construction.production_adjustment)) {
				env.under_construction.production_adjustment =
					production_adjustment_tag(static_cast<production_adjustment_tag::value_base_t>(env.s.technology_m.production_adjustments.outer_size()));
				env.s.technology_m.production_adjustments.resize(to_index(env.under_construction.unit_adjustment) + 1);
			}

			for(const auto& p : v.goods)
				env.s.technology_m.production_adjustments.get(
					env.under_construction.production_adjustment,
					economy_tag_to_production_adjustment<production_adjustment::artisan_goods_throughput>(p.first)) += p.second;
		}
		void set_war_exhaustion(float v) {
			env.under_construction.attributes[tech_offset::war_exhaustion] = v;
		}
		void set_supply_limit(float v) {
			env.under_construction.attributes[tech_offset::supply_limit] = v;
		}
		void set_morale(float v) {
			env.under_construction.attributes[tech_offset::morale] = v;
		}
		void set_prestige(float v) {
			env.under_construction.attributes[tech_offset::prestige] = v;
		}
		void set_combat_width(float v) {
			env.under_construction.attributes[tech_offset::combat_width] = v;
		}
		void set_dig_in_cap(float v) {
			env.under_construction.attributes[tech_offset::dig_in_cap] = v;
		}
		void set_influence(float v) {
			env.under_construction.attributes[tech_offset::influence] = v;
		}
		void set_repair_rate(float v) {
			env.under_construction.attributes[tech_offset::repair_rate] = v;
		}
		void set_reinforce_rate(float v) {
			env.under_construction.attributes[tech_offset::reinforce_rate] = v;
		}
		void set_soldier_to_pop_loss(float v) {
			env.under_construction.attributes[tech_offset::soldier_to_pop_loss] = v;
		}
		void set_regular_experience_level(float v) {
			env.under_construction.attributes[tech_offset::regular_experience_level] = v;
		}
		void set_colonial_life_rating(float v) {
			env.under_construction.attributes[tech_offset::colonial_life_rating] = v;
		}
		void set_education_efficiency(float v) {
			env.under_construction.attributes[tech_offset::education_efficiency] = v;
		}
		void set_military_tactics(float v) {
			env.under_construction.attributes[tech_offset::military_tactics] = v;
		}
		void set_seperatism(float v) {
			env.under_construction.attributes[tech_offset::seperatism] = v;
		}
		void set_land_attrition(float v) {
			env.under_construction.attributes[tech_offset::land_attrition] = v;
		}
		void set_naval_attrition(float v) {
			env.under_construction.attributes[tech_offset::naval_attrition] = v;
		}
		void set_supply_range(float v) {
			env.under_construction.attributes[tech_offset::supply_range] = v;
		}
		void set_gas_attack(bool v) {
			if(v)
				env.under_construction.flags |= technology::gas_attack;
		}
		void set_gas_defence(bool v) {
			if(v)
				env.under_construction.flags |= technology::gas_defence;
		}
		void set_plurality(float v) {
			env.under_construction.attributes[tech_offset::plurality] = v;
		}
		void set_factory_cost(float v) {
			env.under_construction.attributes[tech_offset::factory_cost] = v;
		}
		void set_permanent_prestige(float v) {
			env.under_construction.attributes[tech_offset::permanent_prestige] = v;
		}
		void set_colonial_prestige(float v) {
			env.under_construction.attributes[tech_offset::colonial_prestige] = v;
		}
		void set_colonial_migration(float v) {
			env.under_construction.attributes[tech_offset::colonial_migration] = v;
		}
		void set_colonial_points(float v) {
			env.under_construction.attributes[tech_offset::colonial_points] = v;
		}
		void set_cb_creation_speed(float v) {
			env.under_construction.attributes[tech_offset::cb_creation_speed] = v;
		}
		void set_max_fort(float v) {
			env.under_construction.attributes[tech_offset::max_fort] = v;
		}
		void set_max_naval_base(float v) {
			env.under_construction.attributes[tech_offset::max_naval_base] = v;
		}
		void set_max_railroad(float v) {
			env.under_construction.attributes[tech_offset::max_railroad] = v;
		}
		void pass_modifier(const std::pair<token_and_type, float>& p) {
			env.mod.add_attribute(p);
		}
	};

}

MEMBER_FDEF(technologies::invention_reader, set_effect, "effect");
MEMBER_FDEF(technologies::invention_reader, set_chance, "chance");
MEMBER_FDEF(technologies::invention_reader, set_limit, "limit");
MEMBER_FDEF(technologies::invention_reader, discard, "discard");
MEMBER_FDEF(technologies::invention_reader, pass_modifier, "modifier");
MEMBER_FDEF(technologies::tech_reader, add_subunit, "subunit");
MEMBER_FDEF(technologies::tech_reader, set_year, "year");
MEMBER_FDEF(technologies::tech_reader, set_cost, "cost");
MEMBER_FDEF(technologies::tech_reader, set_ai_chance, "ai_chance");
MEMBER_FDEF(technologies::tech_reader, set_activate_building, "activate_building");
MEMBER_FDEF(technologies::tech_reader, set_enable_crime, "enable_crime");
MEMBER_FDEF(technologies::tech_reader, set_allow, "allow");
MEMBER_FDEF(technologies::tech_reader, set_area, "area");
MEMBER_FDEF(technologies::tech_reader, discard, "discard");
MEMBER_FDEF(technologies::tech_reader, activate_unit, "activate_unit");
MEMBER_FDEF(technologies::tech_reader, deactivate_unit, "deactivate_unit");
MEMBER_FDEF(technologies::tech_reader, set_unciv_military, "unciv_military");
MEMBER_FDEF(technologies::tech_reader, set_shared_prestige, "shared_prestige");
MEMBER_FDEF(technologies::tech_reader, set_pop_growth, "pop_growth");
MEMBER_FDEF(technologies::tech_reader, set_increase_research, "increase_research");
MEMBER_FDEF(technologies::tech_reader, set_administrative_efficiency, "administrative_efficiency");
MEMBER_FDEF(technologies::tech_reader, set_diplomatic_points, "diplomatic_points");
MEMBER_FDEF(technologies::tech_reader, set_tax_eff, "tax_eff");
MEMBER_FDEF(technologies::tech_reader, set_rebel_org_gain, "rebel_org_gain");
MEMBER_FDEF(technologies::tech_reader, set_max_national_focus, "max_national_focus");
MEMBER_FDEF(technologies::tech_reader, set_rgo_size, "rgo_size");
MEMBER_FDEF(technologies::tech_reader, set_factory_goods_output, "factory_goods_output");
MEMBER_FDEF(technologies::tech_reader, set_factory_goods_input, "factory_goods_input");
MEMBER_FDEF(technologies::tech_reader, set_factory_goods_throughput, "factory_goods_throughput");
MEMBER_FDEF(technologies::tech_reader, set_rgo_goods_output, "rgo_goods_output");
MEMBER_FDEF(technologies::tech_reader, set_rgo_goods_throughput, "rgo_goods_throughput");
MEMBER_FDEF(technologies::tech_reader, set_artisan_goods_input, "artisan_goods_input");
MEMBER_FDEF(technologies::tech_reader, set_artisan_goods_output, "artisan_goods_output");
MEMBER_FDEF(technologies::tech_reader, set_artisan_goods_throughput, "artisan_goods_throughput");
MEMBER_FDEF(technologies::tech_reader, set_war_exhaustion, "war_exhaustion");
MEMBER_FDEF(technologies::tech_reader, set_supply_limit, "supply_limit");
MEMBER_FDEF(technologies::tech_reader, set_morale, "morale");
MEMBER_FDEF(technologies::tech_reader, set_prestige, "prestige");
MEMBER_FDEF(technologies::tech_reader, set_combat_width, "combat_width");
MEMBER_FDEF(technologies::tech_reader, set_dig_in_cap, "dig_in_cap");
MEMBER_FDEF(technologies::tech_reader, set_influence, "influence");
MEMBER_FDEF(technologies::tech_reader, set_repair_rate, "repair_rate");
MEMBER_FDEF(technologies::tech_reader, set_reinforce_rate, "reinforce_rate");
MEMBER_FDEF(technologies::tech_reader, set_soldier_to_pop_loss, "soldier_to_pop_loss");
MEMBER_FDEF(technologies::tech_reader, set_regular_experience_level, "regular_experience_level");
MEMBER_FDEF(technologies::tech_reader, set_colonial_life_rating, "colonial_life_rating");
MEMBER_FDEF(technologies::tech_reader, set_education_efficiency, "education_efficiency");
MEMBER_FDEF(technologies::tech_reader, set_military_tactics, "military_tactics");
MEMBER_FDEF(technologies::tech_reader, set_seperatism, "seperatism");
MEMBER_FDEF(technologies::tech_reader, set_land_attrition, "land_attrition");
MEMBER_FDEF(technologies::tech_reader, set_naval_attrition, "naval_attrition");
MEMBER_FDEF(technologies::tech_reader, set_supply_range, "supply_range");
MEMBER_FDEF(technologies::tech_reader, set_gas_attack, "gas_attack");
MEMBER_FDEF(technologies::tech_reader, set_gas_defence, "gas_defence");
MEMBER_FDEF(technologies::tech_reader, set_plurality, "plurality");
MEMBER_FDEF(technologies::tech_reader, set_factory_cost, "factory_cost");
MEMBER_FDEF(technologies::tech_reader, set_permanent_prestige, "permanent_prestige");
MEMBER_FDEF(technologies::tech_reader, set_colonial_prestige, "colonial_prestige");
MEMBER_FDEF(technologies::tech_reader, set_colonial_migration, "colonial_migration");
MEMBER_FDEF(technologies::tech_reader, set_colonial_points, "colonial_points");
MEMBER_FDEF(technologies::tech_reader, set_cb_creation_speed, "cb_creation_speed");
MEMBER_FDEF(technologies::tech_reader, set_max_fort, "max_fort");
MEMBER_FDEF(technologies::tech_reader, set_max_naval_base, "max_naval_base");
MEMBER_FDEF(technologies::tech_reader, set_max_railroad, "max_railroad");
MEMBER_FDEF(technologies::tech_reader, pass_modifier, "modifier");
MEMBER_FDEF(technologies::tech_goods_list, add_good, "good");
MEMBER_FDEF(technologies::tech_subunit, set_attack, "attack_gun_power");
MEMBER_FDEF(technologies::tech_subunit, set_evasion, "maneuver_evasion");
MEMBER_FDEF(technologies::tech_subunit, set_strength, "max_strength");
MEMBER_FDEF(technologies::tech_subunit, set_organization, "default_organisation");
MEMBER_FDEF(technologies::tech_subunit, set_build_time, "build_time");
MEMBER_FDEF(technologies::tech_subunit, set_speed, "maximum_speed");
MEMBER_FDEF(technologies::tech_subunit, set_supply_consumption, "supply_consumption");
MEMBER_FDEF(technologies::tech_subunit, set_hull, "defence_hull");
MEMBER_FDEF(technologies::tech_subunit, set_fire_range, "reconnaissance_fire_range");
MEMBER_FDEF(technologies::tech_subunit, set_discipline, "discipline");
MEMBER_FDEF(technologies::tech_subunit, set_support, "support_torpedo_attack");
MEMBER_FDEF(technologies::tech_subunit, set_siege, "siege");

MEMBER_FDEF(technologies::rebel_org_gain, set_faction, "faction");
MEMBER_DEF(technologies::rebel_org_gain, value, "value");

MEMBER_FDEF(technologies::parse_schools, add_school, "school");
MEMBER_FDEF(technologies::folders, add_folder, "category");
MEMBER_FDEF(technologies::folder, add_sub_category, "sub_category");
MEMBER_FDEF(technologies::technologies_file, handle_folders, "folders");
MEMBER_FDEF(technologies::technologies_file, handle_schools, "schools");
MEMBER_FDEF(technologies::specific_tech_file, insert_tech, "technology");
MEMBER_FDEF(technologies::inventions_pre_parse_file, add_invention, "invention");


namespace technologies {
	using name_mod_pair = std::pair<text_data::text_tag, modifiers::national_modifier_tag>;

	BEGIN_DOMAIN(tech_content_domain)
		BEGIN_TYPE(invention_reader)
		MEMBER_TYPE_ASSOCIATION("effect", "effect", tech_reader)
		MEMBER_ASSOCIATION("discard", "news", discard_from_rh)
		MEMBER_TYPE_EXTERN("limit", "limit", triggers::trigger_tag, read_tech_allow)
		MEMBER_TYPE_EXTERN("chance", "chance", modifiers::factor_tag, read_tech_chance)
		MEMBER_VARIABLE_ASSOCIATION("modifier", accept_all, bind_token_float)
		END_TYPE
		BEGIN_TYPE(tech_reader)
		MEMBER_ASSOCIATION("year", "year", value_from_rh<uint16_t>)
		MEMBER_ASSOCIATION("cost", "cost", value_from_rh<uint16_t>)
		MEMBER_ASSOCIATION("activate_building", "activate_building", token_from_rh)
		MEMBER_ASSOCIATION("enable_crime", "enable_crime", token_from_rh)
		MEMBER_ASSOCIATION("area", "area", token_from_rh)
		MEMBER_ASSOCIATION("discard", "unit", discard_from_rh)
		MEMBER_ASSOCIATION("activate_unit", "activate_unit", token_from_rh)
		MEMBER_ASSOCIATION("deactivate_unit", "deactivate_unit", token_from_rh)
		MEMBER_ASSOCIATION("unciv_military", "unciv_military", value_from_rh<bool>)
		MEMBER_ASSOCIATION("shared_prestige", "shared_prestige", value_from_rh<float>)
		MEMBER_ASSOCIATION("pop_growth", "pop_growth", value_from_rh<float>)
		MEMBER_ASSOCIATION("increase_research", "increase_research", value_from_rh<float>)
		MEMBER_ASSOCIATION("administrative_efficiency", "administrative_efficiency", value_from_rh<float>)
		MEMBER_ASSOCIATION("diplomatic_points", "diplomatic_points", value_from_rh<float>)
		MEMBER_ASSOCIATION("tax_eff", "tax_eff", value_from_rh<float>)
		MEMBER_ASSOCIATION("max_national_focus", "max_national_focus", value_from_rh<float>)
		MEMBER_ASSOCIATION("war_exhaustion", "war_exhaustion", value_from_rh<float>)
		MEMBER_ASSOCIATION("supply_limit", "supply_limit", value_from_rh<float>)
		MEMBER_ASSOCIATION("morale", "morale", value_from_rh<float>)
		MEMBER_ASSOCIATION("prestige", "prestige", value_from_rh<float>)
		MEMBER_ASSOCIATION("combat_width", "combat_width", value_from_rh<float>)
		MEMBER_ASSOCIATION("dig_in_cap", "dig_in_cap", value_from_rh<float>)
		MEMBER_ASSOCIATION("influence", "influence", value_from_rh<float>)
		MEMBER_ASSOCIATION("repair_rate", "repair_rate", value_from_rh<float>)
		MEMBER_ASSOCIATION("reinforce_rate", "reinforce_rate", value_from_rh<float>)
		MEMBER_ASSOCIATION("soldier_to_pop_loss", "soldier_to_pop_loss", value_from_rh<float>)
		MEMBER_ASSOCIATION("regular_experience_level", "regular_experience_level", value_from_rh<float>)
		MEMBER_ASSOCIATION("colonial_life_rating", "colonial_life_rating", value_from_rh<float>)
		MEMBER_ASSOCIATION("education_efficiency", "education_efficiency", value_from_rh<float>)
		MEMBER_ASSOCIATION("military_tactics", "military_tactics", value_from_rh<float>)
		MEMBER_ASSOCIATION("seperatism", "seperatism", value_from_rh<float>)
		MEMBER_ASSOCIATION("land_attrition", "land_attrition", value_from_rh<float>)
		MEMBER_ASSOCIATION("naval_attrition", "naval_attrition", value_from_rh<float>)
		MEMBER_ASSOCIATION("supply_range", "supply_range", value_from_rh<float>)
		MEMBER_ASSOCIATION("gas_attack", "gas_attack", value_from_rh<bool>)
		MEMBER_ASSOCIATION("gas_defence", "gas_defence", value_from_rh<bool>)
		MEMBER_ASSOCIATION("gas_defence", "gas_defense", value_from_rh<bool>)
		MEMBER_ASSOCIATION("plurality", "plurality", value_from_rh<float>)
		MEMBER_ASSOCIATION("factory_cost", "factory_cost", value_from_rh<float>)
		MEMBER_ASSOCIATION("permanent_prestige", "permanent_prestige", value_from_rh<float>)
		MEMBER_ASSOCIATION("colonial_prestige", "colonial_prestige", value_from_rh<float>)
		MEMBER_ASSOCIATION("colonial_migration", "colonial_migration", value_from_rh<float>)
		MEMBER_ASSOCIATION("colonial_points", "colonial_points", value_from_rh<float>)
		MEMBER_ASSOCIATION("cb_creation_speed", "cb_creation_speed", value_from_rh<float>)
		MEMBER_ASSOCIATION("max_fort", "max_fort", value_from_rh<float>)
		MEMBER_ASSOCIATION("max_naval_base", "max_naval_base", value_from_rh<float>)
		MEMBER_ASSOCIATION("max_railroad", "max_railroad", value_from_rh<float>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("subunit", accept_all, tech_subunit, bind_subunit)
		MEMBER_TYPE_ASSOCIATION("rgo_size", "rgo_size", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("factory_goods_output", "factory_goods_output", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("factory_goods_input", "factory_goods_input", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("factory_goods_throughput", "factory_goods_throughput", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("rgo_goods_output", "rgo_goods_output", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("rgo_goods_throughput", "rgo_goods_throughput", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("artisan_goods_input", "artisan_goods_input", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("artisan_goods_output", "artisan_goods_output", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("artisan_goods_throughput", "artisan_goods_throughput", tech_goods_list)
		MEMBER_TYPE_ASSOCIATION("rebel_org_gain", "rebel_org_gain", rebel_org_gain)
		MEMBER_TYPE_EXTERN("allow", "allow", triggers::trigger_tag, read_tech_allow)
		MEMBER_TYPE_EXTERN("ai_chance", "ai_chance", modifiers::factor_tag, read_tech_chance)
		MEMBER_VARIABLE_ASSOCIATION("modifier", accept_all, bind_token_float)
		END_TYPE
		BEGIN_TYPE(tech_goods_list)
		MEMBER_VARIABLE_ASSOCIATION("good", accept_all, bind_token_float)
		END_TYPE
		BEGIN_TYPE(rebel_org_gain)
		MEMBER_ASSOCIATION("value", "value", value_from_rh<float>)
		MEMBER_ASSOCIATION("faction", "faction", token_from_rh)
		END_TYPE
		BEGIN_TYPE(tech_subunit)
		MEMBER_ASSOCIATION("attack_gun_power", "attack", value_from_rh<float>)
		MEMBER_ASSOCIATION("attack_gun_power", "gun_power", value_from_rh<float>)
		MEMBER_ASSOCIATION("maneuver_evasion", "maneuver", value_from_rh<float>)
		MEMBER_ASSOCIATION("maneuver_evasion", "evasion", value_from_rh<float>)
		MEMBER_ASSOCIATION("max_strength", "max_strength", value_from_rh<float>)
		MEMBER_ASSOCIATION("default_organisation", "default_organisation", value_from_rh<float>)
		MEMBER_ASSOCIATION("default_organisation", "default_organization", value_from_rh<float>)
		MEMBER_ASSOCIATION("build_time", "build_time", value_from_rh<float>)
		MEMBER_ASSOCIATION("maximum_speed", "maximum_speed", value_from_rh<float>)
		MEMBER_ASSOCIATION("supply_consumption", "supply_consumption", value_from_rh<float>)
		MEMBER_ASSOCIATION("defence_hull", "defence", value_from_rh<float>)
		MEMBER_ASSOCIATION("defence_hull", "defense", value_from_rh<float>)
		MEMBER_ASSOCIATION("defence_hull", "hull", value_from_rh<float>)
		MEMBER_ASSOCIATION("reconnaissance_fire_range", "reconnaissance", value_from_rh<float>)
		MEMBER_ASSOCIATION("reconnaissance_fire_range", "recon", value_from_rh<float>)
		MEMBER_ASSOCIATION("reconnaissance_fire_range", "fire_range", value_from_rh<float>)
		MEMBER_ASSOCIATION("discipline", "discipline", value_from_rh<float>)
		MEMBER_ASSOCIATION("support_torpedo_attack", "support", value_from_rh<float>)
		MEMBER_ASSOCIATION("support_torpedo_attack", "torpedo_attack", value_from_rh<float>)
		MEMBER_ASSOCIATION("siege", "siege", value_from_rh<float>)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(tech_parsing_domain)
		BEGIN_TYPE(technologies_file)
		MEMBER_TYPE_ASSOCIATION("folders", "folders", folders)
		MEMBER_TYPE_ASSOCIATION("schools", "schools", parse_schools)
		END_TYPE
		BEGIN_TYPE(folders)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("category", accept_all, folder, bind_folder)
		END_TYPE
		BEGIN_TYPE(folder)
		MEMBER_VARIABLE_ASSOCIATION("sub_category", accept_all, token_from_lh)
		END_TYPE
		BEGIN_TYPE(parse_schools)
		MEMBER_VARIABLE_TYPE_EXTERN("school", accept_all, name_mod_pair, read_school_modifier)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(tech_subfile_pre_parsing_domain)
		BEGIN_TYPE(specific_tech_file)
		MEMBER_VARIABLE_TYPE_EXTERN("technology", accept_all, int, inner_pre_parse_tech)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(inventions_pre_parsing_domain)
		BEGIN_TYPE(inventions_pre_parse_file)
		MEMBER_VARIABLE_TYPE_EXTERN("invention", accept_all, int, inner_pre_parse_invention)
		END_TYPE
		END_DOMAIN;

	void pre_parse_single_tech_file(parsing_environment& state, const token_group* start, const token_group* end) {
		parse_object<specific_tech_file, tech_subfile_pre_parsing_domain>(start, end, state);
	}

	void prepare_technologies_read(scenario::scenario_manager& s) {
		s.technology_m.unit_type_adjustments.reset(static_cast<uint32_t>(s.military_m.unit_types.size()));
		s.technology_m.rebel_org_gain.reset(static_cast<uint32_t>(s.population_m.rebel_types.size()));
		s.technology_m.production_adjustments.reset(s.economy_m.goods_count * uint32_t(production_adjustment::production_adjustment_count));
	}

	void read_inventions(parsing_state const& state, scenario::scenario_manager& s) {
		for(const auto& t : state.impl->pending_inventions) {
			technology& uc = s.technology_m.technologies_container[std::get<0>(t)];
			tech_reading_env env(s, uc);
			parse_object<invention_reader, tech_content_domain>(std::get<1>(t), std::get<2>(t), env);
			if(env.mod.total_attributes != 0)
				uc.modifier = modifiers::add_national_modifier(uc.name, env.mod, s.modifiers_m);
		}
	}

	void read_technologies(parsing_state const& state, scenario::scenario_manager& s) {
		for(const auto& t : state.impl->pending_technologies) {
			technology& uc = s.technology_m.technologies_container[std::get<0>(t)];
			tech_reading_env env(s, uc);
			parse_object<tech_reader, tech_content_domain>(std::get<1>(t), std::get<2>(t), env);
			if(env.mod.total_attributes != 0)
				uc.modifier = modifiers::add_national_modifier(uc.name, env.mod, s.modifiers_m);
		}
	}

	void pre_parse_technologies(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		auto& main_results = state.impl->main_file_parse_tree;

		const auto tech_file = common_dir.open_file(u"technology.txt");

		if(tech_file) {
			const auto sz = tech_file->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			tech_file->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<technologies_file, tech_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void pre_parse_inventions(
		parsing_state& state,
		const directory& source_directory) {

		const auto dir = source_directory.get_directory(u"\\inventions");
		const auto files = dir.list_files(u".txt");
		for(const auto& f : files) {
			const auto fi = f.open_file();
			if(fi) {
				state.impl->inventions_parse_trees.emplace_back();
				auto& iparse = state.impl->inventions_parse_trees.back();

				const auto sz = fi->size();
				iparse.parse_data = std::unique_ptr<char[]>(new char[sz]);

				fi->read_to_buffer(iparse.parse_data.get(), sz);
				parse_pdx_file(iparse.parse_results, iparse.parse_data.get(), iparse.parse_data.get() + sz);

				if(iparse.parse_results.size() > 0) {
					parse_object<inventions_pre_parse_file, inventions_pre_parsing_domain>(
						&iparse.parse_results[0],
						&iparse.parse_results[0] + iparse.parse_results.size(),
						*state.impl);
				}
			}
		}
	}
}
