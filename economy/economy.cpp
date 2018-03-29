#include "economy.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "concurrency_tools\\concurrency_tools.hpp"

namespace economy {
	struct parsing_environment {
		text_handle_lookup text_lookup;
		economic_scenario& manager;

		parsing_environment(const text_handle_lookup& tl, economic_scenario& m) :
			text_lookup(tl), manager(m) {
		}
	};

	struct color_builder {
		uint32_t current_color = 0;
		graphics::color_rgb color = { 0,0,0 };

		void add_value(int v) {
			switch (current_color) {
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
		void set_tradeable(bool) {
		}
		void set_money(bool v) {
			if (v)
				def.flags |= good_definition::money;
		}
		void set_available_from_start(bool v) {
			if (!v)
				def.flags |= good_definition::not_available_from_start;
		}
		void set_overseas_penalty(bool v) {
			if (v)
				def.flags |= good_definition::overseas_penalty;
		}
		void set_cost(double v) {
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

		void add_good(const std::pair<token_and_type, good_definition_builder>& v) {
			if ((v.second.def.flags & good_definition::money) != 0) {
				money = v.second.def;
				const auto gname = env.text_lookup(v.first.start, v.first.end);
				money.name = gname;
			} else {
				const auto gname = env.text_lookup(v.first.start, v.first.end);
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
		good_definition money;
		goods_type_tag money_group;

		goods_file(parsing_environment& e) : env(e) {}

		void add_group(const std::pair<token_and_type, goods_group>& v) {
			const auto gname = env.text_lookup(v.first.start, v.first.end);
			const auto new_g_tag = env.manager.good_type_names.emplace_back(gname);
			
			for (auto subgood : v.second.subgoods) {
				env.manager.goods[subgood].type = new_g_tag;
			}

			if (v.second.money.flags != 0) {
				money = v.second.money;
				money_group = new_g_tag;
				money.type = new_g_tag;
			}
		}
	};

	inline std::pair<token_and_type, goods_group> bind_group(const token_and_type& t, association_type, goods_group& f) {
		return std::pair<token_and_type, goods_group>(t, std::move(f));
	}
	inline std::pair<token_and_type, good_definition_builder> bind_good(const token_and_type& t, association_type, good_definition_builder& f) {
		return std::pair<token_and_type, good_definition_builder>(t, std::move(f));
	}

	struct buildings_parsing_environment {
		text_handle_lookup text_lookup;
		economic_scenario& manager;
		boost::container::flat_map<text_data::text_tag, factory_type_tag>& production_to_factory;

		buildings_parsing_environment(const text_handle_lookup& tl, economic_scenario& m, boost::container::flat_map<text_data::text_tag, factory_type_tag>& map) :
			text_lookup(tl), manager(m), production_to_factory(map) {
		}
	};

	enum class building_type_enum {
		infrastructure, factory, fort, naval_base, other
	};
	struct goods_cost_container {
		buildings_parsing_environment& env;
		goods_cost_container(buildings_parsing_environment& e) : env(e) {}

		std::vector<std::pair<goods_tag, double>> cost_pairs;

		void add_cost_pair(const std::pair<token_and_type, double>& p) {
			const auto gtag = env.manager.named_goods_index[env.text_lookup(p.first.start, p.first.end)];
			cost_pairs.emplace_back(gtag, p.second);
		}

	};

	inline std::pair<token_and_type, double> bind_cost_pair(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, double>(l, token_to<double>(r));
	}

	struct building_obj {
		building_type_enum type = building_type_enum::other;
		uint32_t cost;
		std::vector<std::pair<goods_tag, double>> goods_cost;
		uint32_t time;
		uint32_t naval_capacity;
		uint32_t max_level;
		std::vector<int> colonial_points;
		uint32_t colonial_range;
		double local_ship_build;
		double infrastructure;
		double movement_cost;
		text_data::text_tag production_type;
		bool default_enabled = false;
	};

	struct building_obj_container : public building_obj {
		buildings_parsing_environment& env;

		building_obj_container(buildings_parsing_environment& e) : env(e) {}

		void discard_attribute(int) {}

		void set_goods_cost(goods_cost_container& c) {
			goods_cost = std::move(c.cost_pairs);
		}
		void set_colonial_points(std::vector<int>& vec) {
			colonial_points = std::move(vec);
		}
		void set_production_type(const token_and_type& t) {
			production_type = env.text_lookup(t.start, t.end);
		}
		void set_type(const token_and_type& t) {
			if (is_fixed_token_ci(t, "factory")) {
				type = building_type_enum::factory;
			} else if (is_fixed_token_ci(t, "fort")) {
				type = building_type_enum::fort;
			} else if (is_fixed_token_ci(t, "infrastructure")) {
				type = building_type_enum::infrastructure;
			} else if (is_fixed_token_ci(t, "naval_base")) {
				type = building_type_enum::naval_base;
			} else {
				type = building_type_enum::other;
			}
		}
		void discard(int) {}
	};

	inline std::pair<token_and_type, building_obj> bind_building(const token_and_type& t, association_type, building_obj_container& f) {
		return std::pair<token_and_type, building_obj>(t, std::move(f));
	}

	struct buildings_file {
		buildings_parsing_environment& env;
		building_obj fort;
		building_obj railroad;
		building_obj naval_base;

		buildings_file(buildings_parsing_environment& e) : env(e) {
			e.manager.building_costs.reset(static_cast<uint32_t>(e.manager.goods.size()));
		}

		void add_building(std::pair<token_and_type, building_obj>&& b) {
			if (b.second.type == building_type_enum::infrastructure) {
				railroad = std::move(b.second);
			} else if (b.second.type == building_type_enum::fort) {
				fort = std::move(b.second);
			} else if (b.second.type == building_type_enum::naval_base) {
				naval_base = std::move(b.second);
			} else if (b.second.type == building_type_enum::factory) {
				const auto new_ftag = env.manager.factory_types.emplace_back();
				const auto name = env.text_lookup(b.first.start, b.first.end);
				auto& fac = env.manager.factory_types[new_ftag];

				fac.id = new_ftag;
				fac.name = name;
				fac.building_time = b.second.time;
				fac.default_enabled = b.second.default_enabled;
				
				env.production_to_factory.emplace(b.second.production_type, new_ftag);
				env.manager.named_factory_types_index.emplace(name, new_ftag);
				
				env.manager.building_costs.safe_get(new_ftag, goods_tag(0));

				for (const auto& cost : b.second.goods_cost) {
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

			env.manager.building_costs.safe_get(fort_tag, goods_tag(0));
			for (const auto& cost : fort.goods_cost) {
				env.manager.building_costs.get(fort_tag, cost.first) = cost.second;
			}

			env.manager.railroad.cost_tag = railroad_tag;
			env.manager.railroad.infrastructure = static_cast<float>(railroad.infrastructure);
			env.manager.railroad.max_level = railroad.max_level;
			env.manager.railroad.movement_cost = static_cast<float>(railroad.movement_cost);
			env.manager.railroad.time = railroad.time;

			env.manager.building_costs.safe_get(railroad_tag, goods_tag(0));
			for (const auto& cost : railroad.goods_cost) {
				env.manager.building_costs.get(railroad_tag, cost.first) = cost.second;
			}

			env.manager.naval_base.colonial_range = naval_base.colonial_range;
			env.manager.naval_base.cost_tag = naval_base_tag;
			env.manager.naval_base.extra_cost = naval_base.cost;
			env.manager.naval_base.local_ship_build = static_cast<float>(naval_base.local_ship_build);
			env.manager.naval_base.max_level = naval_base.max_level;
			env.manager.naval_base.naval_capacity = naval_base.naval_capacity;
			env.manager.naval_base.time = naval_base.time;

			for (uint32_t i = 0; i < 8 && i < naval_base.colonial_points.size(); ++i) {
				env.manager.naval_base.colonial_points[i] = static_cast<uint32_t>(naval_base.colonial_points[i]);
			}

			env.manager.building_costs.safe_get(naval_base_tag, goods_tag(0));
			for (const auto& cost : naval_base.goods_cost) {
				env.manager.building_costs.get(naval_base_tag, cost.first) = cost.second;
			}
		}
	};
}

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
MEMBER_DEF(economy::building_obj_container, local_ship_build, "local_ship_build");
MEMBER_DEF(economy::building_obj_container, infrastructure, "infrastructure");
MEMBER_DEF(economy::building_obj_container, movement_cost, "movement_cost");
MEMBER_FDEF(economy::building_obj_container, set_production_type, "production_type");
MEMBER_FDEF(economy::building_obj_container, discard, "discard");
MEMBER_DEF(economy::building_obj_container, default_enabled, "default_enabled");
MEMBER_FDEF(economy::goods_cost_container, add_cost_pair, "cost_item");
MEMBER_FDEF(economy::buildings_file, add_building, "building");

namespace economy {
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
		    MEMBER_ASSOCIATION("cost", "cost", value_from_rh<double>)
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
		    MEMBER_ASSOCIATION("discard", "completion_size", discard_from_rh)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("cost", "cost", value_from_rh<uint32_t>)
			MEMBER_ASSOCIATION("time", "time", value_from_rh<uint32_t>)
			MEMBER_ASSOCIATION("naval_capacity", "naval_capacity", value_from_rh<uint32_t>)
			MEMBER_ASSOCIATION("max_level", "max_level", value_from_rh<uint32_t>)
			MEMBER_ASSOCIATION("colonial_range", "colonial_range", value_from_rh<uint32_t>)
			MEMBER_ASSOCIATION("local_ship_build", "local_ship_build", value_from_rh<double>)
			MEMBER_ASSOCIATION("infrastructure", "infrastructure", value_from_rh<double>)
			MEMBER_ASSOCIATION("movement_cost", "movement_cost", value_from_rh<double>)
			MEMBER_ASSOCIATION("production_type", "production_type", token_from_rh)
			MEMBER_ASSOCIATION("default_enabled", "default_enabled", value_from_rh<bool>)
		END_TYPE
	END_DOMAIN;

	void read_goods(
		economic_scenario& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"goods.txt");

		parsing_environment return_state(text_function, manager);

		if (file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if (parse_results.size() > 0) {
				goods_file r = parse_object<goods_file, goods_file_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					return_state);

				if (r.money.flags != 0) {
					const auto money_tag = manager.goods.emplace_back(r.money);
					manager.named_goods_index.emplace(r.money.name, money_tag);
					auto& m = manager.goods[money_tag];
					m.id = money_tag;

					manager.money = money_tag;
				}
			}
		}
	}

	boost::container::flat_map<text_data::text_tag, factory_type_tag> read_buildings(
		economic_scenario& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function
	) {
		boost::container::flat_map<text_data::text_tag, factory_type_tag> production_to_factory;

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"buildings.txt");

		buildings_parsing_environment return_state(text_function, manager, production_to_factory);

		if (file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if (parse_results.size() > 0) {
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
