#include "common\\common.h"
#include "economy_io.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "scenario\\scenario.h"
#include "triggers\\trigger_reading.h"
#include "world_state\\world_state.h"
#include "modifiers\\modifiers_io.h"

void serialization::serializer<economy::economic_state>::deserialize_object(std::byte const *& input, economy::economic_state & obj, world_state & ws) {
	//deserialize(input, obj.current_prices);
	auto coal_name = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, "coal");
	obj.coal = ws.s.economy_m.named_goods_index[coal_name];
}
void serialization::serializer<economy::factory_instance>::serialize_object(std::byte *& output, economy::factory_instance const & obj, world_state const & ws) {
	if(obj.type) {
		auto factory_type = obj.type->id;
		serialize(output, factory_type);
		serialize(output, obj.factory_bank);
		serialize(output, obj.factory_progress);
		serialize(output, obj.worker_data);
		serialize(output, obj.level);
		serialize(output, obj.subsidized);
	} else {
		economy::factory_type_tag factory_type;
		serialize(output, factory_type);
	}
}
void serialization::serializer<economy::factory_instance>::deserialize_object(std::byte const *& input, economy::factory_instance & obj, world_state & ws) {
	economy::factory_type_tag factory_type;
	deserialize(input, factory_type);
	if(is_valid_index(factory_type)) {
		obj.type = &ws.s.economy_m.factory_types[factory_type];
		deserialize(input, obj.factory_bank);
		deserialize(input, obj.factory_progress);
		deserialize(input, obj.worker_data);
		deserialize(input, obj.level);
		deserialize(input, obj.subsidized);
	} else {
		obj.type = nullptr;
		obj.factory_bank = 0.0f;
		obj.worker_data.production_scale = 1.0f;
		obj.level = 0ui16;
	}
}
size_t serialization::serializer<economy::factory_instance>::size(economy::factory_instance const & obj, world_state const & ws) {
	if(obj.type) {
		return sizeof(economy::factory_type_tag) + 
			sizeof(obj.factory_bank) +
			sizeof(obj.factory_progress) +
			serialization::serialize_size(obj.worker_data) +
			sizeof(obj.level) + 
			sizeof(obj.subsidized);
	} else {
		return sizeof(economy::factory_type_tag);
	}
}

namespace economy {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;
		economic_scenario& manager;
		uint8_t icon_count = 0;

		parsing_environment(text_data::text_sequences& tl, economic_scenario& m) :
			text_lookup(tl), manager(m) {}
	};

	struct color_builder {
		uint32_t current_color = 0;
		graphics::color_rgb color = { 0,0,0 };

		void add_value(int v) {
			switch(current_color) {
				case 0:
					color.r = static_cast<uint8_t>(v);
					break;
				case 1:
					color.g = static_cast<uint8_t>(v);
					break;
				case 2:
					color.b = static_cast<uint8_t>(v);
					break;
				default:
					break;
			}
			++current_color;
		}
	};

	struct good_definition_builder {
		good_definition def;
		void set_tradeable(bool) {}
		void set_money(bool v) {
			if(v)
				def.flags |= good_definition::money;
		}
		void set_available_from_start(bool v) {
			if(!v)
				def.flags |= good_definition::not_available_from_start;
		}
		void set_overseas_penalty(bool v) {
			if(v)
				def.flags |= good_definition::overseas_penalty;
		}
		void set_cost(economy::money_qnty_type v) {
			def.base_price = v;
		}
		void set_color(const color_builder& v) {
			def.color = v.color;
		}
	};

	struct goods_group {
		parsing_environment& env;
		good_definition money;
		std::vector<goods_tag> subgoods;

		goods_group(parsing_environment& e) : env(e) {}

		void add_good(std::pair<token_and_type, good_definition_builder>&& v) {
			v.second.def.icon = ++env.icon_count;

			if((v.second.def.flags & good_definition::money) != 0) {
				money = v.second.def;
				const auto gname = text_data::get_thread_safe_text_handle(env.text_lookup, v.first.start, v.first.end);
				money.name = gname;
				env.manager.named_goods_index.emplace(gname, goods_tag(0));
			} else {
				const auto gname = text_data::get_thread_safe_text_handle(env.text_lookup, v.first.start, v.first.end);
				const auto new_g_tag = env.manager.goods.emplace_back(v.second.def);
				auto& new_g = env.manager.goods[new_g_tag];

				new_g.id = new_g_tag;
				new_g.name = gname;
				subgoods.push_back(new_g_tag);

				env.manager.named_goods_index.emplace(gname, new_g_tag);
			}
		}
	};

	struct goods_file {
		parsing_environment& env;

		goods_file(parsing_environment& e) : env(e) {}

		void add_group(const std::pair<token_and_type, goods_group>& v) {
			const auto gname = text_data::get_thread_safe_text_handle(env.text_lookup, v.first.start, v.first.end);
			const auto new_g_tag = env.manager.good_type_names.emplace_back(gname);

			for(auto subgood : v.second.subgoods) {
				env.manager.goods[subgood].type = new_g_tag;
			}

			if(v.second.money.flags != 0) {
				env.manager.goods[goods_tag(0)] = v.second.money;
				env.manager.goods[goods_tag(0)].type = new_g_tag;
				env.manager.goods[goods_tag(0)].id = goods_tag(0);
			}
		}
	};

	inline std::pair<token_and_type, goods_group> bind_group(const token_and_type& t, association_type, goods_group&& f) {
		return std::pair<token_and_type, goods_group>(t, std::move(f));
	}
	inline std::pair<token_and_type, good_definition_builder> bind_good(const token_and_type& t, association_type, good_definition_builder&& f) {
		return std::pair<token_and_type, good_definition_builder>(t, std::move(f));
	}

	struct buildings_parsing_environment {
		text_data::text_sequences& text_lookup;
		economic_scenario& manager;
		boost::container::flat_map<text_data::text_tag, factory_type_tag>& production_to_factory;
		modifiers::modifiers_manager& mod_manager;

		buildings_parsing_environment(text_data::text_sequences& tl, economic_scenario& m, boost::container::flat_map<text_data::text_tag, factory_type_tag>& map, modifiers::modifiers_manager& mm) :
			text_lookup(tl), manager(m), production_to_factory(map), mod_manager(mm) {}
	};

	enum class building_type_enum {
		infrastructure, factory, fort, naval_base, other
	};
	struct goods_cost_container {
		buildings_parsing_environment& env;
		goods_cost_container(buildings_parsing_environment& e) : env(e) {}

		std::vector<std::pair<goods_tag, economy::money_qnty_type>> cost_pairs;

		void add_cost_pair(const std::pair<token_and_type, economy::money_qnty_type>& p) {
			const auto gtag = env.manager.named_goods_index[text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end)];
			cost_pairs.emplace_back(gtag, p.second);
		}

	};

	inline std::pair<token_and_type, economy::money_qnty_type> bind_cost_pair(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, economy::money_qnty_type>(l, token_to<economy::money_qnty_type>(r));
	}

	struct building_obj {
		modifiers::modifier_reading_base building_modifier;
		building_type_enum type = building_type_enum::other;
		uint32_t cost;
		std::vector<std::pair<goods_tag, economy::money_qnty_type>> goods_cost;
		uint32_t time;
		uint32_t naval_capacity;
		uint32_t max_level;
		std::vector<int> colonial_points;
		uint32_t colonial_range;
		double infrastructure;
		text_data::text_tag production_type;
		bool default_enabled = false;
	};

	inline std::pair<token_and_type, float> bind_attribute_pair(const token_and_type& t, association_type, const token_and_type& r) {
		return std::pair<token_and_type, float>(t, token_to<float>(r));
	}

	struct building_obj_container : public building_obj {
		buildings_parsing_environment& env;

		building_obj_container(buildings_parsing_environment& e) : env(e) {}

		void discard_attribute(int) {}

		void set_goods_cost(goods_cost_container&& c) {
			goods_cost = std::move(c.cost_pairs);
		}
		void set_colonial_points(std::vector<int>&& vec) {
			colonial_points = std::move(vec);
		}
		void set_production_type(const token_and_type& t) {
			production_type = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		}
		void set_type(const token_and_type& t) {
			if(is_fixed_token_ci(t, "factory")) {
				type = building_type_enum::factory;
			} else if(is_fixed_token_ci(t, "fort")) {
				type = building_type_enum::fort;
			} else if(is_fixed_token_ci(t, "infrastructure")) {
				type = building_type_enum::infrastructure;
			} else if(is_fixed_token_ci(t, "naval_base")) {
				type = building_type_enum::naval_base;
			} else {
				type = building_type_enum::other;
			}
		}
		void set_other(const std::pair<token_and_type, float>& v) {
			building_modifier.add_attribute(v);
		}
		void discard(int) {}
	};

	inline std::pair<token_and_type, building_obj> bind_building(const token_and_type& t, association_type, building_obj_container&& f) {
		return std::pair<token_and_type, building_obj>(t, std::move(f));
	}

	struct buildings_file {
		buildings_parsing_environment& env;
		building_obj fort;
		building_obj railroad;
		building_obj naval_base;

		buildings_file(buildings_parsing_environment& e) : env(e) {
			e.manager.building_costs.reset(e.manager.aligned_32_goods_count);
		}

		void add_building(std::pair<token_and_type, building_obj>&& b) {
			if(b.second.type == building_type_enum::infrastructure) {
				railroad = std::move(b.second);
			} else if(b.second.type == building_type_enum::fort) {
				fort = std::move(b.second);
			} else if(b.second.type == building_type_enum::naval_base) {
				naval_base = std::move(b.second);
			} else if(b.second.type == building_type_enum::factory) {
				const auto new_ftag = env.manager.factory_types.emplace_back();
				const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, b.first.start, b.first.end);
				auto& fac = env.manager.factory_types[new_ftag];

				fac.id = new_ftag;
				fac.name = name;
				fac.building_time = b.second.time;
				fac.default_enabled = b.second.default_enabled;

				env.production_to_factory.emplace(b.second.production_type, new_ftag);
				env.manager.named_factory_types_index.emplace(name, new_ftag);

				env.manager.building_costs.safe_get(new_ftag, goods_tag(0));

				for(const auto& cost : b.second.goods_cost) {
					env.manager.building_costs.get(new_ftag, cost.first) = cost.second;
				}
			}
		}

		void finalize() {
			const uint8_t num_factories = static_cast<uint8_t>(env.manager.factory_types.size());
			const factory_type_tag fort_tag(num_factories + 0);
			const factory_type_tag railroad_tag(num_factories + 1);
			const factory_type_tag naval_base_tag(num_factories + 2);

			env.manager.fort.cost_tag = fort_tag;
			env.manager.fort.max_level = fort.max_level;
			env.manager.fort.time = fort.time;

			if(int32_t(fort.building_modifier.total_attributes) - int32_t(fort.building_modifier.count_unique_national) > 0)
				env.manager.fort_modifier = modifiers::add_provincial_modifier(text_data::get_thread_safe_text_handle(env.text_lookup, "fort"), fort.building_modifier, env.mod_manager);

			env.manager.building_costs.safe_get(fort_tag, goods_tag(0));
			for(const auto& cost : fort.goods_cost) {
				env.manager.building_costs.get(fort_tag, cost.first) = cost.second;
			}

			env.manager.railroad.cost_tag = railroad_tag;
			env.manager.railroad.infrastructure = static_cast<float>(railroad.infrastructure);
			env.manager.railroad.max_level = railroad.max_level;
			env.manager.railroad.time = railroad.time;

			if(int32_t(railroad.building_modifier.total_attributes) - int32_t(railroad.building_modifier.count_unique_national) > 0)
				env.manager.railroad_modifier = modifiers::add_provincial_modifier(text_data::get_thread_safe_text_handle(env.text_lookup, "railroad"), railroad.building_modifier, env.mod_manager);

			env.manager.building_costs.safe_get(railroad_tag, goods_tag(0));
			for(const auto& cost : railroad.goods_cost) {
				env.manager.building_costs.get(railroad_tag, cost.first) = cost.second;
			}

			env.manager.naval_base.colonial_range = naval_base.colonial_range;
			env.manager.naval_base.cost_tag = naval_base_tag;
			env.manager.naval_base.extra_cost = naval_base.cost;
			env.manager.naval_base.max_level = naval_base.max_level;
			env.manager.naval_base.naval_capacity = naval_base.naval_capacity;
			env.manager.naval_base.time = naval_base.time;

			if(int32_t(naval_base.building_modifier.total_attributes) - int32_t(naval_base.building_modifier.count_unique_national) > 0)
				env.manager.naval_base_modifier = modifiers::add_provincial_modifier(text_data::get_thread_safe_text_handle(env.text_lookup, "naval_base"), naval_base.building_modifier, env.mod_manager);


			for(uint32_t i = 0; i < 8 && i < naval_base.colonial_points.size(); ++i) {
				env.manager.naval_base.colonial_points[i] = static_cast<uint32_t>(naval_base.colonial_points[i]);
			}

			env.manager.building_costs.safe_get(naval_base_tag, goods_tag(0));
			for(const auto& cost : naval_base.goods_cost) {
				env.manager.building_costs.get(naval_base_tag, cost.first) = cost.second;
			}
		}
	};

	enum class production_type_type {
		unknown, rgo, factory, artisan
	};
	struct owner_reader {
		scenario::scenario_manager& env;
		owner_data owner;

		owner_reader(scenario::scenario_manager& e, boost::container::flat_map<text_data::text_tag, factory_type_tag> const&) : env(e) {}

		void set_poptype(const token_and_type& t) {
			owner.type = tag_from_text(
				env.population_m.named_pop_type_index,
				text_data::get_thread_safe_existing_text_handle(env.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_effect(const token_and_type& t) {
			if(is_fixed_token_ci(t, "input"))
				owner.contribution = contribution_type::input;
			else if(is_fixed_token_ci(t, "output"))
				owner.contribution = contribution_type::output;
			else if(is_fixed_token_ci(t, "throughput"))
				owner.contribution = contribution_type::throughput;
		}
		void set_effect_multiplier(float v) {
			owner.effect_multiplier = v;
		}
	};
	struct employee_data_reader {
		scenario::scenario_manager& env;
		employee_data employee;

		employee_data_reader(scenario::scenario_manager& e, boost::container::flat_map<text_data::text_tag, factory_type_tag> const&) : env(e) {}

		void set_poptype(const token_and_type& t) {
			employee.type = tag_from_text(
				env.population_m.named_pop_type_index,
				text_data::get_thread_safe_existing_text_handle(env.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_effect(const token_and_type& t) {
			if(is_fixed_token_ci(t, "input"))
				employee.contribution = contribution_type::input;
			else if(is_fixed_token_ci(t, "output"))
				employee.contribution = contribution_type::output;
			else if(is_fixed_token_ci(t, "throughput"))
				employee.contribution = contribution_type::throughput;
		}
		void set_effect_multiplier(float v) {
			employee.effect_multiplier = v;
		}
		void set_amount(float v) {
			employee.amount = v;
		}
	};

	inline employee_data_reader read_employee(const token_and_type&, association_type, const employee_data_reader& e) {
		return e;
	}

	inline triggers::trigger_tag read_bonus_trigger(const token_group* s, const token_group* e, scenario::scenario_manager& env, boost::container::flat_map<text_data::text_tag, factory_type_tag> const&) {
		const auto td = triggers::parse_trigger(env,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::state,
				triggers::trigger_slot_contents::state,
				triggers::trigger_slot_contents::empty,
				false },
				s, e);
		return triggers::commit_trigger(env.trigger_m, td);
	}
	struct bonus_reader {
		scenario::scenario_manager& env;
		bonus b;

		bonus_reader(scenario::scenario_manager& e, const boost::container::flat_map<text_data::text_tag, factory_type_tag>&) : env(e) {}
		void set_value(float v) {
			b.value = v;
		}
		void set_trigger(triggers::trigger_tag t) {
			b.condition = t;
		}
	};
	inline std::pair<token_and_type, economy::goods_qnty_type> read_econ_pair(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, economy::goods_qnty_type>(l, token_to<economy::goods_qnty_type>(r));
	}
	struct goods_set_reader {
		scenario::scenario_manager& env;
		std::vector<std::pair<goods_tag, economy::goods_qnty_type>> data;
		goods_set_reader(scenario::scenario_manager& e, const boost::container::flat_map<text_data::text_tag, factory_type_tag>&) : env(e) {}

		void add_value(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto gt = tag_from_text(
				env.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(gt))
				data.emplace_back(gt, p.second);
		}
	};
	struct employee_set {
		scenario::scenario_manager& env;
		employee_data workers[max_worker_types];

		employee_set(scenario::scenario_manager& e, boost::container::flat_map<text_data::text_tag, factory_type_tag>const &) : env(e) {}

		void add_employee(const employee_data_reader& v) {
			for(uint32_t i = 0; i < std::extent_v<decltype(workers)>; ++i) {
				if(!is_valid_index(workers[i].type)) {
					workers[i] = v.employee;
					break;
				}
			}
		}
	};

	struct production_type_reader {
		scenario::scenario_manager& env;

		text_data::text_tag template_name;
		bool mine = false;
		bool farm = false;
		bool is_coastal = false;
		owner_data owner;
		employee_data workers[max_worker_types];
		production_type_type type = production_type_type::unknown;
		std::vector<std::pair<goods_tag, economy::goods_qnty_type>> efficiency_goods;
		std::vector<std::pair<goods_tag, economy::goods_qnty_type>> input_goods;
		economy::goods_qnty_type value = economy::goods_qnty_type(1);
		goods_tag output_good;
		int32_t workforce = 0;
		bonus bonuses[std::extent_v<decltype(factory_type::bonuses)>];

		production_type_reader(scenario::scenario_manager& e, const boost::container::flat_map<text_data::text_tag, factory_type_tag>&) : env(e) {}

		void set_owner(const owner_reader& v) {
			owner = v.owner;
		}
		void set_employees(const employee_set& v) {
			for(uint32_t i = 0; i < std::extent_v<decltype(workers)>; ++i)
				workers[i] = v.workers[i];
		}
		void set_bonus(const bonus_reader& v) {
			for(uint32_t i = 0; i < std::extent_v<decltype(bonuses)>; ++i) {
				if(!is_valid_index(bonuses[i].condition)) {
					bonuses[i] = v.b;
					break;
				}
			}
		}
		void set_template(const token_and_type& t) {
			template_name = text_data::get_thread_safe_text_handle(env.gui_m.text_data_sequences, t.start, t.end);
		}
		void set_output_goods(const token_and_type& t) {
			output_good = tag_from_text(
				env.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_efficiency(goods_set_reader&& v) {
			efficiency_goods = std::move(v.data);
		}
		void set_input_goods(goods_set_reader&& v) {
			input_goods = std::move(v.data);
		}
		void set_type(const token_and_type& t) {
			if(is_fixed_token_ci(t, "rgo"))
				type = production_type_type::rgo;
			else if(is_fixed_token_ci(t, "factory"))
				type = production_type_type::factory;
			else if(is_fixed_token_ci(t, "artisan"))
				type = production_type_type::artisan;
		}
	};

	inline std::pair<token_and_type, production_type_reader> read_pt(const token_and_type& t, association_type, production_type_reader&& pt) {
		return std::pair<token_and_type, production_type_reader>(t, std::move(pt));
	}

	struct production_reading {
		scenario::scenario_manager& env;
		boost::container::flat_map<text_data::text_tag, factory_type_tag>& factory_mapping;
		text_data::text_tag RGO_template_farmers;
		text_data::text_tag RGO_template_labourers;

		boost::container::flat_map<text_data::text_tag, artisan_type_tag> artisan_templates;

		production_reading(scenario::scenario_manager& e, boost::container::flat_map<text_data::text_tag, factory_type_tag>& m) : env(e), factory_mapping(m) {
			static const char t1[] = "RGO_template_farmers";
			static const char t2[] = "RGO_template_labourers";

			RGO_template_farmers = text_data::get_thread_safe_text_handle(env.gui_m.text_data_sequences, t1, t1 + sizeof(t1) - 1);
			RGO_template_labourers = text_data::get_thread_safe_text_handle(env.gui_m.text_data_sequences, t2, t2 + sizeof(t2) - 1);
		}

		void add_item(const std::pair<token_and_type, production_type_reader>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.gui_m.text_data_sequences, p.first.start, p.first.end);

			const bool factory_by_inference = is_valid_index(p.second.template_name) && factory_mapping.count(p.second.template_name) != 0;
			const bool artisan_by_inference = is_valid_index(p.second.template_name) && artisan_templates.count(p.second.template_name) != 0;
			const bool rgo_by_inference = p.second.template_name == RGO_template_farmers || p.second.template_name == RGO_template_labourers;

			if(p.second.type == production_type_type::factory || factory_by_inference) {
				const auto pre_mapped_ftag = tag_from_text(factory_mapping, name);
				const auto ftag = is_valid_index(pre_mapped_ftag) ? pre_mapped_ftag : env.economy_m.factory_types.emplace_back();
				factory_type& factory = env.economy_m.factory_types[ftag];

				factory.factory_workers.workforce = p.second.workforce;
				factory.factory_workers.owner = p.second.owner;
				for(uint32_t i = 0; i < std::extent_v<decltype(factory.factory_workers.workers)>; ++i)
					factory.factory_workers.workers[i] = p.second.workers[i];

				if(is_valid_index(p.second.template_name)) {
					const auto base_tag = tag_from_text(factory_mapping, p.second.template_name);
					const auto old_name = factory.name;
					const auto old_building_time = factory.building_time;
					const auto old_default_enabled = factory.default_enabled;

					if(is_valid_index(base_tag)) {
						factory = env.economy_m.factory_types[base_tag];

						Eigen::Map<const Eigen::VectorXf, Eigen::AlignmentType::Aligned32> source_vector(
							env.economy_m.factory_efficiency_goods.safe_get_row(base_tag), env.economy_m.aligned_32_goods_count);
						Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32> dest_vector(
							env.economy_m.factory_efficiency_goods.safe_get_row(ftag), env.economy_m.aligned_32_goods_count);

						dest_vector = source_vector;
					}
					factory.name = old_name;
					factory.building_time = old_building_time;
					factory.default_enabled = old_default_enabled;
				}

				factory.id = ftag;
				for(uint32_t i = 0; i < std::extent_v<decltype(factory.bonuses)>; ++i)
					factory.bonuses[i] = p.second.bonuses[i];
				factory.coastal = p.second.is_coastal;
				factory.output_amount = p.second.value;
				factory.output_good = p.second.output_good;

				if(is_valid_index(p.second.output_good))
					env.economy_m.goods[p.second.output_good].factory_id = ftag;

				for(const auto& i : p.second.input_goods)
					env.economy_m.factory_input_goods.safe_get(ftag, i.first) = i.second;
				for(const auto& i : p.second.efficiency_goods)
					env.economy_m.factory_efficiency_goods.safe_get(ftag, i.first) = i.second;

				factory_mapping.emplace(name, ftag);
			} else if(p.second.type == production_type_type::artisan || artisan_by_inference) {
				const auto atag = env.economy_m.artisan_types.emplace_back();
				artisan_type& artisan = env.economy_m.artisan_types[atag];

				artisan.workforce = p.second.workforce;
				artisan.artisan_contribution = p.second.owner.contribution;

				if(is_valid_index(p.second.template_name)) {
					const auto base_tag = tag_from_text(artisan_templates, p.second.template_name);
					if(is_valid_index(base_tag))
						artisan = env.economy_m.artisan_types[base_tag];
				}

				artisan.id = atag;
				artisan.coastal = p.second.is_coastal;
				artisan.name = name;
				artisan.output_amount = p.second.value;
				artisan.output_good = p.second.output_good;

				if(is_valid_index(p.second.output_good))
					env.economy_m.goods[p.second.output_good].artisan_id = atag;

				for(const auto& i : p.second.input_goods)
					env.economy_m.artisan_input_goods.safe_get(atag, i.first) = i.second;

				artisan_templates.emplace(name, atag);
			} else if(p.second.type == production_type_type::rgo || rgo_by_inference) {
				if(name == RGO_template_farmers) {
					env.economy_m.rgo_farm.owner = p.second.owner;
					for(uint32_t i = 0; i < std::extent_v<decltype(env.economy_m.rgo_farm.workers)>; ++i)
						env.economy_m.rgo_farm.workers[i] = p.second.workers[i];
					env.economy_m.rgo_farm.workforce = p.second.workforce;
				} else if(name == RGO_template_labourers) {
					env.economy_m.rgo_mine.owner = p.second.owner;
					for(uint32_t i = 0; i < std::extent_v<decltype(env.economy_m.rgo_mine.workers)>; ++i)
						env.economy_m.rgo_mine.workers[i] = p.second.workers[i];
					env.economy_m.rgo_mine.workforce = p.second.workforce;
				} else if(is_valid_index(p.second.output_good)) {
					good_definition& g = env.economy_m.goods[p.second.output_good];
					g.rgo_name = name;
					g.base_rgo_value = p.second.value;
					if(p.second.mine)
						g.flags |= good_definition::mined;
					g.flags |= good_definition::has_rgo;
				}
			}
		}
	};
}

MEMBER_FDEF(economy::production_reading, add_item, "item");
MEMBER_FDEF(economy::employee_set, add_employee, "employee");
MEMBER_FDEF(economy::production_type_reader, set_type, "type");
MEMBER_DEF(economy::production_type_reader, value, "value");
MEMBER_DEF(economy::production_type_reader, mine, "mine");
MEMBER_DEF(economy::production_type_reader, farm, "farm");
MEMBER_DEF(economy::production_type_reader, is_coastal, "is_coastal");
MEMBER_DEF(economy::production_type_reader, workforce, "workforce");
MEMBER_FDEF(economy::production_type_reader, set_owner, "owner");
MEMBER_FDEF(economy::production_type_reader, set_employees, "employees");
MEMBER_FDEF(economy::production_type_reader, set_bonus, "bonus");
MEMBER_FDEF(economy::production_type_reader, set_template, "template");
MEMBER_FDEF(economy::production_type_reader, set_output_goods, "output_goods");
MEMBER_FDEF(economy::production_type_reader, set_efficiency, "efficiency");
MEMBER_FDEF(economy::production_type_reader, set_input_goods, "input_goods");
MEMBER_FDEF(economy::owner_reader, set_poptype, "poptype");
MEMBER_FDEF(economy::owner_reader, set_effect, "effect");
MEMBER_FDEF(economy::owner_reader, set_effect_multiplier, "effect_multiplier");
MEMBER_FDEF(economy::employee_data_reader, set_poptype, "poptype");
MEMBER_FDEF(economy::employee_data_reader, set_effect, "effect");
MEMBER_FDEF(economy::employee_data_reader, set_effect_multiplier, "effect_multiplier");
MEMBER_FDEF(economy::employee_data_reader, set_amount, "amount");
MEMBER_FDEF(economy::bonus_reader, set_value, "value");
MEMBER_FDEF(economy::bonus_reader, set_trigger, "trigger");
MEMBER_FDEF(economy::goods_set_reader, add_value, "value");

MEMBER_FDEF(economy::color_builder, add_value, "color");
MEMBER_FDEF(economy::goods_file, add_group, "group");
MEMBER_FDEF(economy::goods_group, add_good, "good");
MEMBER_FDEF(economy::good_definition_builder, set_tradeable, "tradeable");
MEMBER_FDEF(economy::good_definition_builder, set_money, "money");
MEMBER_FDEF(economy::good_definition_builder, set_available_from_start, "available_from_start");
MEMBER_FDEF(economy::good_definition_builder, set_overseas_penalty, "overseas_penalty");
MEMBER_FDEF(economy::good_definition_builder, set_cost, "cost");
MEMBER_FDEF(economy::good_definition_builder, set_color, "color");
MEMBER_DEF(economy::building_obj_container, cost, "cost");
MEMBER_DEF(economy::building_obj_container, time, "time");
MEMBER_FDEF(economy::building_obj_container, set_type, "type");
MEMBER_FDEF(economy::building_obj_container, set_goods_cost, "goods_cost");
MEMBER_DEF(economy::building_obj_container, naval_capacity, "naval_capacity");
MEMBER_DEF(economy::building_obj_container, max_level, "max_level");
MEMBER_FDEF(economy::building_obj_container, set_colonial_points, "colonial_points");
MEMBER_DEF(economy::building_obj_container, colonial_range, "colonial_range");
MEMBER_DEF(economy::building_obj_container, infrastructure, "infrastructure");
MEMBER_FDEF(economy::building_obj_container, set_production_type, "production_type");
MEMBER_FDEF(economy::building_obj_container, set_other, "other");
MEMBER_FDEF(economy::building_obj_container, discard, "discard");
MEMBER_DEF(economy::building_obj_container, default_enabled, "default_enabled");
MEMBER_FDEF(economy::goods_cost_container, add_cost_pair, "cost_item");
MEMBER_FDEF(economy::buildings_file, add_building, "building");

namespace economy {
	BEGIN_DOMAIN(production_domain)
		BEGIN_TYPE(production_type_reader)
		MEMBER_ASSOCIATION("type", "type", token_from_rh)
		MEMBER_ASSOCIATION("value", "value", value_from_rh<economy::goods_qnty_type>)
		MEMBER_ASSOCIATION("mine", "mine", value_from_rh<bool>)
		MEMBER_ASSOCIATION("farm", "farm", value_from_rh<bool>)
		MEMBER_ASSOCIATION("is_coastal", "is_coastal", value_from_rh<bool>)
		MEMBER_ASSOCIATION("workforce", "workforce", value_from_rh<int32_t>)
		MEMBER_TYPE_ASSOCIATION("owner", "owner", owner_reader)
		MEMBER_TYPE_ASSOCIATION("employees", "employees", employee_set)
		MEMBER_TYPE_ASSOCIATION("bonus", "bonus", bonus_reader)
		MEMBER_ASSOCIATION("template", "template", token_from_rh)
		MEMBER_ASSOCIATION("output_goods", "output_goods", token_from_rh)
		MEMBER_TYPE_ASSOCIATION("efficiency", "efficiency", goods_set_reader)
		MEMBER_TYPE_ASSOCIATION("input_goods", "input_goods", goods_set_reader)
		END_TYPE
		BEGIN_TYPE(production_reading)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("item", accept_all, production_type_reader, read_pt)
		END_TYPE
		BEGIN_TYPE(goods_set_reader)
		MEMBER_VARIABLE_ASSOCIATION("value", accept_all, read_econ_pair)
		END_TYPE
		BEGIN_TYPE(bonus_reader)
		MEMBER_ASSOCIATION("value", "value", value_from_rh<float>)
		MEMBER_TYPE_EXTERN("trigger", "trigger", triggers::trigger_tag, read_bonus_trigger)
		END_TYPE
		BEGIN_TYPE(employee_data_reader)
		MEMBER_ASSOCIATION("poptype", "poptype", token_from_rh)
		MEMBER_ASSOCIATION("effect_multiplier", "effect_multiplier", value_from_rh<float>)
		MEMBER_ASSOCIATION("amount", "amount", value_from_rh<float>)
		MEMBER_ASSOCIATION("effect", "effect", token_from_rh)
		END_TYPE
		BEGIN_TYPE(owner_reader)
		MEMBER_ASSOCIATION("poptype", "poptype", token_from_rh)
		MEMBER_ASSOCIATION("effect_multiplier", "effect_multiplier", value_from_rh<float>)
		MEMBER_ASSOCIATION("effect", "effect", token_from_rh)
		END_TYPE
		BEGIN_TYPE(employee_set)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("employee", accept_all, employee_data_reader, read_employee)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(goods_file_domain)
		BEGIN_TYPE(goods_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("group", accept_all, goods_group, bind_group)
		END_TYPE
		BEGIN_TYPE(goods_group)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("good", accept_all, good_definition_builder, bind_good)
		END_TYPE
		BEGIN_TYPE(good_definition_builder)
		MEMBER_ASSOCIATION("tradeable", "tradeable", value_from_rh<bool>)
		MEMBER_ASSOCIATION("money", "money", value_from_rh<bool>)
		MEMBER_ASSOCIATION("available_from_start", "available_from_start", value_from_rh<bool>)
		MEMBER_ASSOCIATION("overseas_penalty", "overseas_penalty", value_from_rh<bool>)
		MEMBER_ASSOCIATION("cost", "cost", value_from_rh<economy::money_qnty_type>)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		END_TYPE
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(buildings_file_domain)
		BEGIN_TYPE(std::vector<int>)
		MEMBER_VARIABLE_ASSOCIATION("this", accept_all, value_from_lh<int>)
		END_TYPE
		BEGIN_TYPE(buildings_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("building", accept_all, building_obj_container, bind_building)
		END_TYPE
		BEGIN_TYPE(goods_cost_container)
		MEMBER_VARIABLE_ASSOCIATION("cost_item", accept_all, bind_cost_pair)
		END_TYPE
		BEGIN_TYPE(building_obj_container)
		MEMBER_TYPE_ASSOCIATION("goods_cost", "goods_cost", goods_cost_container)
		MEMBER_TYPE_ASSOCIATION("colonial_points", "colonial_points", std::vector<int>)
		MEMBER_ASSOCIATION("discard", "port", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "visibility", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "onmap", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "capital", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "province", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "fort_level", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "pop_build_factory", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "spawn_railway_track", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "strategic_factory", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "advanced_factory", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "on_completion", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "sail", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "steam", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "completion_size", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "one_per_state", discard_from_rh)
		MEMBER_ASSOCIATION("type", "type", token_from_rh)
		MEMBER_ASSOCIATION("cost", "cost", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("time", "time", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("naval_capacity", "naval_capacity", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("max_level", "max_level", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("colonial_range", "colonial_range", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("infrastructure", "infrastructure", value_from_rh<double>)
		MEMBER_ASSOCIATION("production_type", "production_type", token_from_rh)
		MEMBER_ASSOCIATION("default_enabled", "default_enabled", value_from_rh<bool>)
		MEMBER_VARIABLE_ASSOCIATION("other", accept_all, bind_attribute_pair)
		END_TYPE
		END_DOMAIN;

	void read_goods(
		economic_scenario& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"goods.txt");

		parsing_environment return_state(text_function, manager);

		if(file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			manager.goods.resize(1ui64);

			if(parse_results.size() > 0) {
				parse_object<goods_file, goods_file_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					return_state);
			}
		}

		manager.goods_count = static_cast<uint32_t>(manager.goods.size());
		manager.aligned_32_goods_count = ((static_cast<uint32_t>(sizeof(goods_qnty_type)) * manager.goods_count + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(goods_qnty_type));

		manager.factory_efficiency_goods.reset(manager.aligned_32_goods_count);
		manager.factory_input_goods.reset(manager.aligned_32_goods_count);
		manager.artisan_input_goods.reset(manager.aligned_32_goods_count);
	}

	void read_production_types(scenario::scenario_manager& s, boost::container::flat_map<text_data::text_tag, factory_type_tag>& map, const directory& source_directory) {
		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"production_types.txt");

		if(file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(parse_results.size() > 0) {
				parse_object<production_reading, production_domain>(
					parse_results.data(),
					parse_results.data() + parse_results.size(),
					s, map);
			}
		}

		for(int32_t i = int32_t(s.economy_m.factory_types.size()) - 1; i >= 0; --i) {
			if(!is_valid_index(s.economy_m.factory_types[factory_type_tag(static_cast<factory_type_tag::value_base_t>(i))].output_good))
				s.economy_m.factory_types.pop_back();
			else
				break;
		}

		s.economy_m.factory_input_goods.resize(s.economy_m.factory_types.size());
		s.economy_m.factory_efficiency_goods.resize(s.economy_m.factory_types.size());
		s.economy_m.artisan_input_goods.resize(s.economy_m.artisan_types.size());
	}

	boost::container::flat_map<text_data::text_tag, factory_type_tag> read_buildings(
		economic_scenario& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function,
		modifiers::modifiers_manager& mod_manager
	) {
		boost::container::flat_map<text_data::text_tag, factory_type_tag> production_to_factory;

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"buildings.txt");

		buildings_parsing_environment return_state(text_function, manager, production_to_factory, mod_manager);

		if(file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(parse_results.size() > 0) {
				auto r = parse_object<buildings_file, buildings_file_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					return_state);

				r.finalize();
			}
		}

		return production_to_factory;
	}
}
