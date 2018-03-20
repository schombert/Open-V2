#include "economy.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace economy {
	struct parsing_environment {
		text_handle_lookup text_lookup;
		economic_scenario& manager;

		parsed_data main_file_parse_tree;

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
}
